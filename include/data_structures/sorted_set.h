#ifndef SORTED_SET_H
#define SORTED_SET_H

#include "data_structures/avl_tree.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <mutex>
#include <optional>

namespace my_redis {

// Sorted set implementation using AVL tree and hash map
class SortedSet {
public:
    SortedSet();
    ~SortedSet() = default;
    
    // Disable copy and move
    SortedSet(const SortedSet&) = delete;
    SortedSet& operator=(const SortedSet&) = delete;
    SortedSet(SortedSet&&) = delete;
    SortedSet& operator=(SortedSet&&) = delete;
    
    // Core operations
    bool add(const std::string& member, double score);
    bool remove(const std::string& member);
    std::optional<double> get_score(const std::string& member) const;
    
    // Range operations
    std::vector<std::pair<std::string, double>> range_by_score(double min_score, double max_score) const;
    std::vector<std::pair<std::string, double>> range_by_rank(size_t start, size_t stop) const;
    
    // Utility functions
    size_t size() const;
    bool empty() const;
    void clear();
    
    // Rank operations
    std::optional<size_t> rank(const std::string& member) const;
    std::optional<std::string> get_by_rank(size_t rank) const;
    
    // Get all members with scores
    std::vector<std::pair<std::string, double>> get_all() const;

private:
    // Score-to-member AVL tree for range queries
    AVLTree<double, std::string> score_tree_;
    
    // Member-to-score hash map for O(1) lookups
    std::unordered_map<std::string, double> member_scores_;
    
    // Mutex for thread safety
    mutable std::mutex mutex_;
};

} // namespace my_redis

#endif // SORTED_SET_H 