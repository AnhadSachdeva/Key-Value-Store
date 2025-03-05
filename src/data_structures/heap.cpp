#include "data_structures/heap.h"

namespace my_redis {

TTLHeap::TTLHeap() = default;

void TTLHeap::add(const std::string& key, TimePoint expiry) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    // Check if key already exists
    auto it = key_to_index_.find(key);
    if (it != key_to_index_.end()) {
        // Update existing entry
        size_t index = it->second;
        heap_[index].expiry = expiry;
        heapify_up(index);
        heapify_down(index);
        return;
    }
    
    // Add new entry
    heap_.emplace_back(key, expiry);
    size_t index = heap_.size() - 1;
    key_to_index_[key] = index;
    heapify_up(index);
}

void TTLHeap::update(const std::string& key, TimePoint expiry) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    auto it = key_to_index_.find(key);
    if (it != key_to_index_.end()) {
        size_t index = it->second;
        TimePoint old_expiry = heap_[index].expiry;
        heap_[index].expiry = expiry;
        
        // Heapify up or down based on the new expiry
        if (expiry < old_expiry) {
            heapify_up(index);
        } else {
            heapify_down(index);
        }
    } else {
        // Key doesn't exist, add it
        add(key, expiry);
    }
}

void TTLHeap::remove(const std::string& key) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    auto it = key_to_index_.find(key);
    if (it == key_to_index_.end()) {
        return;  // Key not found
    }
    
    size_t index = it->second;
    size_t last_index = heap_.size() - 1;
    
    // If it's not the last element, swap with the last one
    if (index != last_index) {
        swap_entries(index, last_index);
        
        // Remove the last element (which is now the one we want to remove)
        heap_.pop_back();
        key_to_index_.erase(key);
        
        // Heapify to maintain heap property
        if (!heap_.empty()) {
            heapify_down(index);
        }
    } else {
        // It's the last element, just remove it
        heap_.pop_back();
        key_to_index_.erase(key);
    }
}

std::vector<std::string> TTLHeap::get_expired_keys() {
    std::lock_guard<std::mutex> lock(mutex_);
    std::vector<std::string> expired_keys;
    
    auto now = std::chrono::steady_clock::now();
    
    // Extract all expired keys
    while (!heap_.empty() && heap_[0].expiry <= now) {
        expired_keys.push_back(heap_[0].key);
        
        // Remove the expired key
        std::string key = heap_[0].key;
        size_t last_index = heap_.size() - 1;
        
        swap_entries(0, last_index);
        heap_.pop_back();
        key_to_index_.erase(key);
        
        if (!heap_.empty()) {
            heapify_down(0);
        }
    }
    
    return expired_keys;
}

size_t TTLHeap::size() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return heap_.size();
}

bool TTLHeap::empty() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return heap_.empty();
}

void TTLHeap::clear() {
    std::lock_guard<std::mutex> lock(mutex_);
    heap_.clear();
    key_to_index_.clear();
}

std::optional<TTLHeap::TimePoint> TTLHeap::next_expiry() const {
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (heap_.empty()) {
        return std::nullopt;
    }
    
    return heap_[0].expiry;
}

void TTLHeap::heapify_up(size_t index) {
    while (index > 0) {
        size_t parent = (index - 1) / 2;
        
        if (heap_[index] < heap_[parent]) {
            swap_entries(index, parent);
            index = parent;
        } else {
            break;
        }
    }
}

void TTLHeap::heapify_down(size_t index) {
    size_t size = heap_.size();
    
    while (true) {
        size_t left_child = 2 * index + 1;
        size_t right_child = 2 * index + 2;
        size_t smallest = index;
        
        if (left_child < size && heap_[left_child] < heap_[smallest]) {
            smallest = left_child;
        }
        
        if (right_child < size && heap_[right_child] < heap_[smallest]) {
            smallest = right_child;
        }
        
        if (smallest != index) {
            swap_entries(index, smallest);
            index = smallest;
        } else {
            break;
        }
    }
}

void TTLHeap::swap_entries(size_t i, size_t j) {
    std::swap(heap_[i], heap_[j]);
    key_to_index_[heap_[i].key] = i;
    key_to_index_[heap_[j].key] = j;
}

} // namespace my_redis 