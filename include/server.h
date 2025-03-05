#ifndef SERVER_H
#define SERVER_H

#include "storage_engine.h"
#include "commands/command_handler.h"
#include <string>
#include <vector>
#include <thread>
#include <atomic>
#include <mutex>
#include <unordered_map>
#include <functional>
#include <memory>

namespace my_redis {

// Server class
class Server {
public:
    Server(int port = 6379);
    ~Server();
    
    // Disable copy and move
    Server(const Server&) = delete;
    Server& operator=(const Server&) = delete;
    Server(Server&&) = delete;
    Server& operator=(Server&&) = delete;
    
    // Start/stop the server
    bool start();
    void stop();
    
    // Process a command directly (for testing)
    std::string process_command(const std::string& command_str);

private:
    int port_;
    int server_fd_;
    std::atomic<bool> running_;
    
    // Storage engine
    StorageEngine storage_;
    
    // Command handler
    CommandHandler command_handler_;
    
    // Thread pool
    std::vector<std::thread> worker_threads_;
    
    // Client connections
    struct ClientConnection {
        int fd;
        std::string buffer;
        
        ClientConnection(int fd_) : fd(fd_) {}
    };
    
    std::unordered_map<int, std::unique_ptr<ClientConnection>> clients_;
    std::mutex clients_mutex_;
    
    // Event loop
    void event_loop();
    
    // Handle client connections
    void handle_new_connection(int kq);
    void handle_client_data(int client_fd);
    void close_client(int client_fd, int kq);
    
    // Process commands
    void process_client_buffer(ClientConnection& client);
    
    // Send response to client
    void send_response(int client_fd, const std::string& response);
};

} // namespace my_redis

#endif // SERVER_H 