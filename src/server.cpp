#include "server.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/event.h>  // kqueue
#include <iostream>
#include <cstring>
#include <thread>
#include <algorithm>

namespace my_redis {

// Helper function to set socket to non-blocking mode
void set_nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

Server::Server(int port)
    : port_(port), server_fd_(-1), running_(false), command_handler_(storage_) {}

Server::~Server() {
    stop();
}

bool Server::start() {
    if (running_) {
        return false;  // Already running
    }
    
    // Create socket
    server_fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd_ < 0) {
        std::cerr << "Failed to create socket" << std::endl;
        return false;
    }
    
    // Set socket options
    int opt = 1;
    if (setsockopt(server_fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        std::cerr << "Failed to set socket options" << std::endl;
        close(server_fd_);
        return false;
    }
    
    // Set non-blocking mode
    set_nonblocking(server_fd_);
    
    // Bind socket
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port_);
    
    if (bind(server_fd_, (struct sockaddr*)&address, sizeof(address)) < 0) {
        std::cerr << "Failed to bind socket" << std::endl;
        close(server_fd_);
        return false;
    }
    
    // Listen for connections
    if (listen(server_fd_, SOMAXCONN) < 0) {
        std::cerr << "Failed to listen on socket" << std::endl;
        close(server_fd_);
        return false;
    }
    
    // Start event loop
    running_ = true;
    std::thread event_thread(&Server::event_loop, this);
    worker_threads_.push_back(std::move(event_thread));
    
    std::cout << "Server started on port " << port_ << std::endl;
    return true;
}

void Server::stop() {
    if (!running_) {
        return;
    }
    
    running_ = false;
    
    // Close server socket
    if (server_fd_ >= 0) {
        close(server_fd_);
        server_fd_ = -1;
    }
    
    // Close all client connections
    {
        std::lock_guard<std::mutex> lock(clients_mutex_);
        for (auto& [fd, client] : clients_) {
            close(fd);
        }
        clients_.clear();
    }
    
    // Join worker threads
    for (auto& thread : worker_threads_) {
        if (thread.joinable()) {
            thread.join();
        }
    }
    worker_threads_.clear();
    
    std::cout << "Server stopped" << std::endl;
}

std::string Server::process_command(const std::string& command_str) {
    auto args = CommandHandler::parse_command(command_str);
    auto response = command_handler_.process_command(args);
    return response.serialize();
}

void Server::event_loop() {
    // Create kqueue instance
    int kq = kqueue();
    if (kq < 0) {
        std::cerr << "Failed to create kqueue instance" << std::endl;
        return;
    }
    
    // Add server socket to kqueue
    struct kevent change;
    EV_SET(&change, server_fd_, EVFILT_READ, EV_ADD, 0, 0, NULL);
    if (kevent(kq, &change, 1, NULL, 0, NULL) < 0) {
        std::cerr << "Failed to add server socket to kqueue" << std::endl;
        close(kq);
        return;
    }
    
    // Event loop
    const int MAX_EVENTS = 64;
    struct kevent events[MAX_EVENTS];
    
    while (running_) {
        // Get next expiry time for TTL
        auto next_expiry = storage_.next_expiry_ms();
        
        // Set timeout for kevent
        struct timespec timeout;
        struct timespec* timeout_ptr = NULL;
        
        if (next_expiry.has_value()) {
            auto ms = next_expiry.value().count();
            timeout.tv_sec = ms / 1000;
            timeout.tv_nsec = (ms % 1000) * 1000000;
            timeout_ptr = &timeout;
        }
        
        // Wait for events
        int num_events = kevent(kq, NULL, 0, events, MAX_EVENTS, timeout_ptr);
        
        if (num_events < 0) {
            if (errno == EINTR) {
                continue;  // Interrupted by signal
            }
            std::cerr << "kevent error: " << strerror(errno) << std::endl;
            break;
        }
        
        // Process expired keys
        storage_.process_expired_keys();
        
        // Process events
        for (int i = 0; i < num_events; ++i) {
            int fd = events[i].ident;
            
            if (fd == server_fd_) {
                // New connection
                handle_new_connection(kq);
            } else {
                // Client data
                if (events[i].filter == EVFILT_READ) {
                    handle_client_data(fd);
                }
                
                if (events[i].flags & (EV_EOF | EV_ERROR)) {
                    close_client(fd, kq);
                }
            }
        }
    }
    
    close(kq);
}

void Server::handle_new_connection(int kq) {
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    
    int client_fd = accept(server_fd_, (struct sockaddr*)&client_addr, &client_len);
    if (client_fd < 0) {
        if (errno != EAGAIN && errno != EWOULDBLOCK) {
            std::cerr << "Failed to accept connection: " << strerror(errno) << std::endl;
        }
        return;
    }
    
    // Set non-blocking mode
    set_nonblocking(client_fd);
    
    // Add to kqueue
    struct kevent change;
    EV_SET(&change, client_fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
    if (kevent(kq, &change, 1, NULL, 0, NULL) < 0) {
        std::cerr << "Failed to add client socket to kqueue" << std::endl;
        close(client_fd);
        return;
    }
    
    // Add to clients map
    {
        std::lock_guard<std::mutex> lock(clients_mutex_);
        clients_[client_fd] = std::make_unique<ClientConnection>(client_fd);
    }
    
    std::cout << "New client connected: " << client_fd << std::endl;
}

void Server::handle_client_data(int client_fd) {
    std::unique_ptr<ClientConnection> client;
    
    {
        std::lock_guard<std::mutex> lock(clients_mutex_);
        auto it = clients_.find(client_fd);
        if (it == clients_.end()) {
            return;  // Client not found
        }
        client = std::move(it->second);
        clients_.erase(it);
    }
    
    if (!client) {
        return;
    }
    
    // Read data from client
    char buffer[4096];
    ssize_t bytes_read;
    
    while ((bytes_read = read(client_fd, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytes_read] = '\0';
        client->buffer += buffer;
        
        // Process commands in buffer
        process_client_buffer(*client);
    }
    
    if (bytes_read < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
        // No more data to read
        std::lock_guard<std::mutex> lock(clients_mutex_);
        clients_[client_fd] = std::move(client);
    } else {
        // Error or connection closed
        close(client_fd);
    }
}

void Server::close_client(int client_fd, int kq) {
    // Remove from kqueue
    struct kevent change;
    EV_SET(&change, client_fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
    kevent(kq, &change, 1, NULL, 0, NULL);
    
    // Remove from clients map and close
    std::lock_guard<std::mutex> lock(clients_mutex_);
    clients_.erase(client_fd);
    close(client_fd);
    std::cout << "Client disconnected: " << client_fd << std::endl;
}

void Server::process_client_buffer(ClientConnection& client) {
    // Simple command processing for now
    // In a real Redis, we would parse the RESP protocol
    
    size_t pos = client.buffer.find("\r\n");
    while (pos != std::string::npos) {
        std::string command = client.buffer.substr(0, pos);
        client.buffer.erase(0, pos + 2);
        
        if (!command.empty()) {
            std::string response = process_command(command);
            send_response(client.fd, response);
        }
        
        pos = client.buffer.find("\r\n");
    }
}

void Server::send_response(int client_fd, const std::string& response) {
    ssize_t bytes_sent = send(client_fd, response.c_str(), response.size(), 0);
    if (bytes_sent < 0) {
        std::cerr << "Failed to send response: " << strerror(errno) << std::endl;
    }
}

} // namespace my_redis 