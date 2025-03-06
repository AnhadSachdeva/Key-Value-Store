#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include "include/storage_engine.h"

using namespace my_redis;
using namespace std::chrono_literals;

int main() {
    // Create a storage engine
    StorageEngine storage;
    
    // Start the expiry thread
    storage.start_expiry_thread();
    
    std::cout << "Setting keys with different expiration times...\n";
    
    // Set keys with different expiration times
    storage.set_ex("key1", "value1", 2s);  // Expires in 2 seconds
    storage.set_ex("key2", "value2", 5s);  // Expires in 5 seconds
    storage.set("key3", "value3");         // No expiration
    
    // Check initial state
    std::cout << "Initial state:\n";
    std::cout << "key1 exists: " << (storage.exists("key1") ? "yes" : "no") << "\n";
    std::cout << "key2 exists: " << (storage.exists("key2") ? "yes" : "no") << "\n";
    std::cout << "key3 exists: " << (storage.exists("key3") ? "yes" : "no") << "\n";
    std::cout << "DB size: " << storage.dbsize() << "\n\n";
    
    // Wait for key1 to expire
    std::cout << "Waiting 3 seconds for key1 to expire...\n";
    std::this_thread::sleep_for(3s);
    
    // Check state after key1 expires
    std::cout << "After 3 seconds:\n";
    std::cout << "key1 exists: " << (storage.exists("key1") ? "yes" : "no") << "\n";
    std::cout << "key2 exists: " << (storage.exists("key2") ? "yes" : "no") << "\n";
    std::cout << "key3 exists: " << (storage.exists("key3") ? "yes" : "no") << "\n";
    std::cout << "DB size: " << storage.dbsize() << "\n\n";
    
    // Wait for key2 to expire
    std::cout << "Waiting 3 more seconds for key2 to expire...\n";
    std::this_thread::sleep_for(3s);
    
    // Check final state
    std::cout << "After 6 seconds total:\n";
    std::cout << "key1 exists: " << (storage.exists("key1") ? "yes" : "no") << "\n";
    std::cout << "key2 exists: " << (storage.exists("key2") ? "yes" : "no") << "\n";
    std::cout << "key3 exists: " << (storage.exists("key3") ? "yes" : "no") << "\n";
    std::cout << "DB size: " << storage.dbsize() << "\n\n";
    
    // Test updating expiry
    std::cout << "Setting key3 to expire in 2 seconds...\n";
    storage.expire("key3", 2s);
    
    // Wait for key3 to expire
    std::cout << "Waiting 3 seconds for key3 to expire...\n";
    std::this_thread::sleep_for(3s);
    
    // Check if key3 expired
    std::cout << "After key3 should expire:\n";
    std::cout << "key3 exists: " << (storage.exists("key3") ? "yes" : "no") << "\n";
    std::cout << "DB size: " << storage.dbsize() << "\n\n";
    
    // Stop the expiry thread
    storage.stop_expiry_thread();
    
    std::cout << "Test completed.\n";
    return 0;
} 