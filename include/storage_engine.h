#ifndef STORAGE_ENGINE_H
#define STORAGE_ENGINE_H

#include "data_structures/hash_table.h"
#include "data_structures/heap.h"
#include "data_structures/sorted_set.h"
#include <string>
#include <chrono>
#include <optional>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>

namespace my_redis {

// Main storage engine class
class StorageEngine {
public:
    StorageEngine();
    ~StorageEngine();
    
    // Disable copy and move
    StorageEngine(const StorageEngine&) = delete;
    StorageEngine& operator=(const StorageEngine&) = delete;
    StorageEngine(StorageEngine&&) = delete;
    StorageEngine& operator=(StorageEngine&&) = delete;
    
    // Core operations
    bool set(const std::string& key, const std::string& value);
    bool set_nx(const std::string& key, const std::string& value);
    bool set_ex(const std::string& key, const std::string& value, std::chrono::seconds ttl);
    std::optional<std::string> get(const std::string& key);
    bool del(const std::string& key);
    bool exists(const std::string& key);
    bool expire(const std::string& key, std::chrono::seconds ttl);
    std::optional<std::chrono::seconds> ttl(const std::string& key);
    
    // Sorted set operations
    bool zadd(const std::string& key, const std::string& member, double score);
    bool zrem(const std::string& key, const std::string& member);
    std::optional<double> zscore(const std::string& key, const std::string& member);
    std::vector<std::pair<std::string, double>> zrange(const std::string& key, size_t start, size_t stop);
    std::vector<std::pair<std::string, double>> zrangebyscore(const std::string& key, double min, double max);
    std::optional<size_t> zrank(const std::string& key, const std::string& member);
    size_t zcard(const std::string& key);
    
    // Utility functions
    size_t dbsize() const;
    void flushdb();
    
    // For TTL management
    std::optional<std::chrono::milliseconds> next_expiry_ms() const;
    void process_expired_keys();
    
    // Start/stop the TTL expiry thread
    void start_expiry_thread();
    void stop_expiry_thread();

private:
    HashTable hash_table_;
    TTLHeap ttl_heap_;
    
    // Thread for TTL expiry
    std::thread expiry_thread_;
    std::atomic<bool> running_;
    std::mutex expiry_mutex_;
    std::condition_variable expiry_cv_;
    
    // Helper methods
    SortedSet* get_sorted_set(const std::string& key, bool create_if_missing = false);
};

} // namespace my_redis

#endif // STORAGE_ENGINE_H 