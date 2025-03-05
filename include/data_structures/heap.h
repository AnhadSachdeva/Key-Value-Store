#ifndef HEAP_H
#define HEAP_H

#include <vector>
#include <string>
#include <chrono>
#include <functional>
#include <mutex>
#include <unordered_map>

namespace my_redis {

// Min-heap implementation for TTL management
class TTLHeap {
public:
    using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;
    
    struct TTLEntry {
        std::string key;
        TimePoint expiry;
        
        TTLEntry(const std::string& k, TimePoint exp)
            : key(k), expiry(exp) {}
        
        bool operator<(const TTLEntry& other) const {
            return expiry < other.expiry;
        }
        
        bool operator>(const TTLEntry& other) const {
            return expiry > other.expiry;
        }
    };
    
    TTLHeap();
    ~TTLHeap() = default;
    
    // Disable copy and move
    TTLHeap(const TTLHeap&) = delete;
    TTLHeap& operator=(const TTLHeap&) = delete;
    TTLHeap(TTLHeap&&) = delete;
    TTLHeap& operator=(TTLHeap&&) = delete;
    
    // Core operations
    void add(const std::string& key, TimePoint expiry);
    void update(const std::string& key, TimePoint expiry);
    void remove(const std::string& key);
    
    // Get expired keys (keys with expiry <= now)
    std::vector<std::string> get_expired_keys();
    
    // Utility functions
    size_t size() const;
    bool empty() const;
    void clear();
    
    // Get the next expiry time (for select/epoll timeout)
    std::optional<TimePoint> next_expiry() const;

private:
    std::vector<TTLEntry> heap_;
    std::unordered_map<std::string, size_t> key_to_index_;
    mutable std::mutex mutex_;
    
    // Heap operations
    void heapify_up(size_t index);
    void heapify_down(size_t index);
    void swap_entries(size_t i, size_t j);
};

} // namespace my_redis

#endif // HEAP_H 