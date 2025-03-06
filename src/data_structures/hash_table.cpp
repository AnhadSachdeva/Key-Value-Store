#include "data_structures/hash_table.h"
#include <algorithm>
#include <stdexcept>

namespace my_redis {

// Value methods
std::string Value::as_string() const {
    switch (type_) {
        case Type::STRING:
            return string_value_;
        case Type::INTEGER:
            return std::to_string(int_value_);
        case Type::SORTED_SET:
            return "[SORTED_SET]";
        case Type::NONE:
            return "[NONE]";
        default:
            return "[UNKNOWN]";
    }
}

int64_t Value::as_int() const {
    switch (type_) {
        case Type::INTEGER:
            return int_value_;
        case Type::STRING:
            try {
                return std::stoll(string_value_);
            } catch (const std::exception&) {
                throw std::runtime_error("Cannot convert string to integer");
            }
        default:
            throw std::runtime_error("Cannot convert to integer");
    }
}

// Entry methods
bool Entry::is_expired() const {
    if (!expiry.has_value()) {
        return false;
    }
    return std::chrono::steady_clock::now() > expiry.value();
}

// HashTable methods
HashTable::HashTable(size_t initial_size)
    : bucket_count_(initial_size), buckets_(initial_size), size_(0) {}

HashTable::~HashTable() {
    clear();
}

size_t HashTable::hash_function(const std::string& key) const {
    // Simple hash function using std::hash
    return std::hash<std::string>{}(key) % bucket_count_;
}

bool HashTable::set(const std::string& key, const Value& value) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    size_t bucket_idx = hash_function(key);
    auto it = find_entry(key, bucket_idx);
    
    if (it != buckets_[bucket_idx].end()) {
        // Update existing entry
        it->value = value;
        it->expiry = std::nullopt;  // Reset expiry
        return true;
    } else {
        // Insert new entry
        buckets_[bucket_idx].emplace_back(key, value);
        size_++;
        resize_if_needed();
        return true;
    }
}

bool HashTable::set_with_expiry(const std::string& key, const Value& value, 
                               std::chrono::seconds ttl) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    size_t bucket_idx = hash_function(key);
    auto it = find_entry(key, bucket_idx);
    
    auto expiry_time = std::chrono::steady_clock::now() + ttl;
    
    if (it != buckets_[bucket_idx].end()) {
        // Update existing entry
        it->value = value;
        it->expiry = expiry_time;
        return true;
    } else {
        // Insert new entry
        buckets_[bucket_idx].emplace_back(key, value, expiry_time);
        size_++;
        resize_if_needed();
        return true;
    }
}

std::optional<Value> HashTable::get(const std::string& key) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    size_t bucket_idx = hash_function(key);
    auto it = find_entry(key, bucket_idx);
    
    if (it != buckets_[bucket_idx].end()) {
        return it->value;
    }
    
    return std::nullopt;
}

bool HashTable::del(const std::string& key) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    size_t bucket_idx = hash_function(key);
    auto it = find_entry(key, bucket_idx);
    
    if (it != buckets_[bucket_idx].end()) {
        buckets_[bucket_idx].erase(it);
        size_--;
        return true;
    }
    
    return false;
}

bool HashTable::exists(const std::string& key) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    size_t bucket_idx = hash_function(key);
    auto it = find_entry(key, bucket_idx);
    
    return it != buckets_[bucket_idx].end();
}

bool HashTable::expire(const std::string& key, std::chrono::seconds ttl) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    size_t bucket_idx = hash_function(key);
    auto it = find_entry(key, bucket_idx);
    
    if (it != buckets_[bucket_idx].end()) {
        it->expiry = std::chrono::steady_clock::now() + ttl;
        return true;
    }
    
    return false;
}

size_t HashTable::size() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return size_;
}

void HashTable::clear() {
    std::lock_guard<std::mutex> lock(mutex_);
    for (auto& bucket : buckets_) {
        bucket.clear();
    }
    size_ = 0;
}

void HashTable::for_each(const std::function<void(const std::string&, const Value&)>& fn) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    for (auto& bucket : buckets_) {
        for (auto it = bucket.begin(); it != bucket.end(); ++it) {
            fn(it->key, it->value);
        }
    }
}

void HashTable::resize_if_needed() {
    // Resize if load factor exceeds 0.75
    if (size_ > bucket_count_ * 0.75) {
        resize(bucket_count_ * 2);
    }
}

void HashTable::resize(size_t new_size) {
    std::vector<std::list<Entry>> new_buckets(new_size);
    
    for (auto& bucket : buckets_) {
        for (auto& entry : bucket) {
            size_t new_idx = std::hash<std::string>{}(entry.key) % new_size;
            new_buckets[new_idx].push_back(entry);
        }
    }
    
    bucket_count_ = new_size;
    buckets_ = std::move(new_buckets);
}

std::list<Entry>::iterator HashTable::find_entry(const std::string& key, size_t bucket_idx) {
    return std::find_if(buckets_[bucket_idx].begin(), buckets_[bucket_idx].end(),
                       [&key](const Entry& entry) { return entry.key == key; });
}

std::list<Entry>::const_iterator HashTable::find_entry(const std::string& key, size_t bucket_idx) const {
    return std::find_if(buckets_[bucket_idx].begin(), buckets_[bucket_idx].end(),
                       [&key](const Entry& entry) { return entry.key == key; });
}

} // namespace my_redis 