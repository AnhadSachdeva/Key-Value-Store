#include "storage_engine.h"
#include <algorithm>

namespace my_redis {

StorageEngine::StorageEngine() : running_(false) {
    start_expiry_thread();
}

StorageEngine::~StorageEngine() {
    stop_expiry_thread();
}

bool StorageEngine::set(const std::string& key, const std::string& value) {
    return hash_table_.set(key, Value(value));
}

bool StorageEngine::set_nx(const std::string& key, const std::string& value) {
    if (hash_table_.exists(key)) {
        return false;
    }
    return hash_table_.set(key, Value(value));
}

bool StorageEngine::set_ex(const std::string& key, const std::string& value, std::chrono::seconds ttl) {
    auto result = hash_table_.set_with_expiry(key, Value(value), ttl);
    
    if (result) {
        auto expiry = std::chrono::steady_clock::now() + ttl;
        ttl_heap_.add(key, expiry);
        expiry_cv_.notify_one();
    }
    
    return result;
}

std::optional<std::string> StorageEngine::get(const std::string& key) {
    auto value_opt = hash_table_.get(key);
    
    if (value_opt.has_value()) {
        const Value& value = value_opt.value();
        if (value.is_string()) {
            return value.as_string();
        }
    }
    
    return std::nullopt;
}

bool StorageEngine::del(const std::string& key) {
    bool result = hash_table_.del(key);
    
    if (result) {
        ttl_heap_.remove(key);
    }
    
    return result;
}

bool StorageEngine::exists(const std::string& key) {
    return hash_table_.exists(key);
}

bool StorageEngine::expire(const std::string& key, std::chrono::seconds ttl) {
    if (!hash_table_.exists(key)) {
        return false;
    }
    
    bool result = hash_table_.expire(key, ttl);
    
    if (result) {
        auto expiry = std::chrono::steady_clock::now() + ttl;
        ttl_heap_.add(key, expiry);
        expiry_cv_.notify_one();
    }
    
    return result;
}

std::optional<std::chrono::seconds> StorageEngine::ttl(const std::string& key) {
    // This is a simplified implementation
    // In a real Redis, we would store the expiry time with the key
    // and calculate the remaining time
    
    if (!hash_table_.exists(key)) {
        return std::nullopt;
    }
    
    // For now, just return a default value
    return std::chrono::seconds(0);
}

bool StorageEngine::zadd(const std::string& key, const std::string& member, double score) {
    SortedSet* zset = get_sorted_set(key, true);
    if (!zset) {
        return false;
    }
    
    return zset->add(member, score);
}

bool StorageEngine::zrem(const std::string& key, const std::string& member) {
    SortedSet* zset = get_sorted_set(key);
    if (!zset) {
        return false;
    }
    
    return zset->remove(member);
}

std::optional<double> StorageEngine::zscore(const std::string& key, const std::string& member) {
    SortedSet* zset = get_sorted_set(key);
    if (!zset) {
        return std::nullopt;
    }
    
    return zset->get_score(member);
}

std::vector<std::pair<std::string, double>> StorageEngine::zrange(const std::string& key, size_t start, size_t stop) {
    SortedSet* zset = get_sorted_set(key);
    if (!zset) {
        return {};
    }
    
    return zset->range_by_rank(start, stop);
}

std::vector<std::pair<std::string, double>> StorageEngine::zrangebyscore(const std::string& key, double min, double max) {
    SortedSet* zset = get_sorted_set(key);
    if (!zset) {
        return {};
    }
    
    return zset->range_by_score(min, max);
}

std::optional<size_t> StorageEngine::zrank(const std::string& key, const std::string& member) {
    SortedSet* zset = get_sorted_set(key);
    if (!zset) {
        return std::nullopt;
    }
    
    return zset->rank(member);
}

size_t StorageEngine::zcard(const std::string& key) {
    SortedSet* zset = get_sorted_set(key);
    if (!zset) {
        return 0;
    }
    
    return zset->size();
}

size_t StorageEngine::dbsize() const {
    return hash_table_.size();
}

void StorageEngine::flushdb() {
    hash_table_.clear();
    ttl_heap_.clear();
}

std::optional<std::chrono::milliseconds> StorageEngine::next_expiry_ms() const {
    auto next_expiry = ttl_heap_.next_expiry();
    
    if (!next_expiry.has_value()) {
        return std::nullopt;
    }
    
    auto now = std::chrono::steady_clock::now();
    if (next_expiry.value() <= now) {
        return std::chrono::milliseconds(0);
    }
    
    return std::chrono::duration_cast<std::chrono::milliseconds>(next_expiry.value() - now);
}

void StorageEngine::process_expired_keys() {
    auto expired_keys = ttl_heap_.get_expired_keys();
    
    for (const auto& key : expired_keys) {
        hash_table_.del(key);
    }
}

void StorageEngine::start_expiry_thread() {
    if (running_) {
        return;
    }
    
    running_ = true;
    expiry_thread_ = std::thread([this]() {
        while (running_) {
            {
                std::unique_lock<std::mutex> lock(expiry_mutex_);
                
                auto next_expiry = next_expiry_ms();
                if (next_expiry.has_value()) {
                    // Wait until next expiry or notification
                    expiry_cv_.wait_for(lock, next_expiry.value());
                } else {
                    // No expiry, wait for notification
                    expiry_cv_.wait(lock);
                }
            }
            
            // Process expired keys
            process_expired_keys();
        }
    });
}

void StorageEngine::stop_expiry_thread() {
    if (!running_) {
        return;
    }
    
    running_ = false;
    expiry_cv_.notify_one();
    
    if (expiry_thread_.joinable()) {
        expiry_thread_.join();
    }
}

SortedSet* StorageEngine::get_sorted_set(const std::string& key, bool create_if_missing) {
    auto value_opt = hash_table_.get(key);
    
    if (value_opt.has_value()) {
        Value& value = const_cast<Value&>(value_opt.value());
        
        if (value.is_sorted_set()) {
            return static_cast<SortedSet*>(value.get_sorted_set_ptr());
        } else if (create_if_missing) {
            // Key exists but is not a sorted set
            return nullptr;
        }
    } else if (create_if_missing) {
        // Create new sorted set
        auto* zset = new SortedSet();
        Value value;
        value.set_sorted_set_ptr(zset);
        hash_table_.set(key, value);
        return zset;
    }
    
    return nullptr;
}

} // namespace my_redis 