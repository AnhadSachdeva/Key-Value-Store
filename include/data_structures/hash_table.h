#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <string>
#include <vector>
#include <list>
#include <memory>
#include <optional>
#include <functional>
#include <mutex>
#include <chrono>

namespace my_redis {


// Value class to store different types of data
class Value {
public:
    enum class Type {
        STRING,
        INTEGER,
        SORTED_SET,
        NONE
    };

    Value() : type_(Type::NONE) {}
    explicit Value(const std::string& str) : type_(Type::STRING), string_value_(str) {}
    explicit Value(int64_t num) : type_(Type::INTEGER), int_value_(num) {}
    
    Type type() const { return type_; }
    
    std::string as_string() const;
    int64_t as_int() const;
    
    bool is_string() const { return type_ == Type::STRING; }
    bool is_int() const { return type_ == Type::INTEGER; }
    bool is_sorted_set() const { return type_ == Type::SORTED_SET; }
    
    // For sorted set operations
    void* get_sorted_set_ptr() const { return sorted_set_ptr_; }
    void set_sorted_set_ptr(void* ptr) { 
        sorted_set_ptr_ = ptr; 
        type_ = Type::SORTED_SET;
    }

private:
    Type type_;
    std::string string_value_;
    int64_t int_value_ = 0;
    void* sorted_set_ptr_ = nullptr;
};

// Entry in the hash table
struct Entry {
    std::string key;
    Value value;
    std::optional<std::chrono::time_point<std::chrono::steady_clock>> expiry;
    
    Entry(const std::string& k, const Value& v) 
        : key(k), value(v), expiry(std::nullopt) {}
    
    Entry(const std::string& k, const Value& v, 
          std::chrono::time_point<std::chrono::steady_clock> exp) 
        : key(k), value(v), expiry(exp) {}
    
    bool is_expired() const;
};

// Custom hash table implementation
class HashTable {
public:
    explicit HashTable(size_t initial_size = 1024);
    ~HashTable();
    
    // Disable copy and move
    HashTable(const HashTable&) = delete;
    HashTable& operator=(const HashTable&) = delete;
    HashTable(HashTable&&) = delete;
    HashTable& operator=(HashTable&&) = delete;
    
    // Core operations
    bool set(const std::string& key, const Value& value);
    bool set_with_expiry(const std::string& key, const Value& value, 
                         std::chrono::seconds ttl);
    std::optional<Value> get(const std::string& key);
    bool del(const std::string& key);
    bool exists(const std::string& key);
    bool expire(const std::string& key, std::chrono::seconds ttl);
    
    // Utility functions
    size_t size() const;
    void clear();
    
    // For iteration
    void for_each(const std::function<void(const std::string&, const Value&)>& fn);
    
private:
    size_t bucket_count_;
    std::vector<std::list<Entry>> buckets_;
    mutable std::mutex mutex_;
    size_t size_;
    
    size_t hash_function(const std::string& key) const;
    void resize_if_needed();
    void resize(size_t new_size);
    
    // Find an entry in a bucket
    std::list<Entry>::iterator find_entry(const std::string& key, size_t bucket_idx);
    std::list<Entry>::const_iterator find_entry(const std::string& key, size_t bucket_idx) const;
};

} // namespace my_redis

#endif // HASH_TABLE_H 