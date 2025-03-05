#include "data_structures/sorted_set.h"
#include <algorithm>

namespace my_redis {

SortedSet::SortedSet() = default;

bool SortedSet::add(const std::string& member, double score) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    auto it = member_scores_.find(member);
    if (it != member_scores_.end()) {
        // Member already exists, update score if different
        if (it->second != score) {
            // Remove old score from tree
            score_tree_.remove(it->second);
            
            // Update score
            it->second = score;
            score_tree_.insert(score, member);
            return true;
        }
        return false;  // Score unchanged
    } else {
        // New member
        member_scores_[member] = score;
        score_tree_.insert(score, member);
        return true;
    }
}

bool SortedSet::remove(const std::string& member) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    auto it = member_scores_.find(member);
    if (it != member_scores_.end()) {
        // Remove from score tree
        score_tree_.remove(it->second);
        
        // Remove from member map
        member_scores_.erase(it);
        return true;
    }
    
    return false;  // Member not found
}

std::optional<double> SortedSet::get_score(const std::string& member) const {
    std::lock_guard<std::mutex> lock(mutex_);
    
    auto it = member_scores_.find(member);
    if (it != member_scores_.end()) {
        return it->second;
    }
    
    return std::nullopt;
}

std::vector<std::pair<std::string, double>> SortedSet::range_by_score(double min_score, double max_score) const {
    std::lock_guard<std::mutex> lock(mutex_);
    
    // Get range from AVL tree
    auto range = score_tree_.range(min_score, max_score);
    
    // Convert to member-score pairs
    std::vector<std::pair<std::string, double>> result;
    result.reserve(range.size());
    
    for (const auto& [score, member] : range) {
        result.emplace_back(member, score);
    }
    
    return result;
}

std::vector<std::pair<std::string, double>> SortedSet::range_by_rank(size_t start, size_t stop) const {
    std::lock_guard<std::mutex> lock(mutex_);
    
    // Get all elements in sorted order
    auto all = score_tree_.get_all();
    
    // Adjust indices
    if (start >= all.size()) {
        return {};
    }
    
    stop = std::min(stop, all.size() - 1);
    
    // Extract the requested range
    std::vector<std::pair<std::string, double>> result;
    result.reserve(stop - start + 1);
    
    for (size_t i = start; i <= stop; ++i) {
        result.emplace_back(all[i].second, all[i].first);
    }
    
    return result;
}

size_t SortedSet::size() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return member_scores_.size();
}

bool SortedSet::empty() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return member_scores_.empty();
}

void SortedSet::clear() {
    std::lock_guard<std::mutex> lock(mutex_);
    member_scores_.clear();
    score_tree_.clear();
}

std::optional<size_t> SortedSet::rank(const std::string& member) const {
    std::lock_guard<std::mutex> lock(mutex_);
    
    auto it = member_scores_.find(member);
    if (it == member_scores_.end()) {
        return std::nullopt;
    }
    
    double score = it->second;
    
    // Get all elements in sorted order
    auto all = score_tree_.get_all();
    
    // Find the position of the member
    for (size_t i = 0; i < all.size(); ++i) {
        if (all[i].first == score && all[i].second == member) {
            return i;
        }
    }
    
    return std::nullopt;  // Should not happen
}

std::optional<std::string> SortedSet::get_by_rank(size_t rank) const {
    std::lock_guard<std::mutex> lock(mutex_);
    
    // Get all elements in sorted order
    auto all = score_tree_.get_all();
    
    if (rank >= all.size()) {
        return std::nullopt;
    }
    
    return all[rank].second;
}

std::vector<std::pair<std::string, double>> SortedSet::get_all() const {
    std::lock_guard<std::mutex> lock(mutex_);
    
    // Get all elements in sorted order
    auto all = score_tree_.get_all();
    
    // Convert to member-score pairs
    std::vector<std::pair<std::string, double>> result;
    result.reserve(all.size());
    
    for (const auto& [score, member] : all) {
        result.emplace_back(member, score);
    }
    
    return result;
}

} // namespace my_redis 