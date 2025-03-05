#include "server.h"
#include <iostream>
#include <csignal>
#include <cstdlib>

using namespace my_redis;

// Global server instance for signal handling
Server* g_server = nullptr;
volatile sig_atomic_t g_shutdown = 0;

// Signal handler
void signal_handler(int signal) {
    if (g_server) {
        std::cout << "Received signal " << signal << ", shutting down..." << std::endl;
        g_server->stop();
        g_shutdown = 1;
    }
}

int main(int argc, char* argv[]) {
    // Parse command line arguments
    int port = 6379;  // Default Redis port
    
    if (argc > 1) {
        port = std::atoi(argv[1]);
        if (port <= 0 || port > 65535) {
            std::cerr << "Invalid port number. Using default port 6379." << std::endl;
            port = 6379;
        }
    }
    
    // Create server
    Server server(port);
    g_server = &server;
    
    // Set up signal handlers
    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);
    
    // Start server
    if (!server.start()) {
        std::cerr << "Failed to start server" << std::endl;
        return 1;
    }
    
    // Wait for signals
    while (!g_shutdown) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    return 0;
} 