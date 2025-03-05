#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <memory>
#include <functional>
#include <vector>
#include <optional>

namespace my_redis {

template <typename K, typename V>
class AVLTree {
private:
    struct Node {
        K key;
        V value;
        int height;
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;
        
        Node(const K& k, const V& v)
            : key(k), value(v), height(1), left(nullptr), right(nullptr) {}
    };
    
    std::unique_ptr<Node> root_;
    size_t size_;
    
    // Helper functions
    int height(const Node* node) const {
        return node ? node->height : 0;
    }
    
    int balance_factor(const Node* node) const {
        return node ? height(node->left.get()) - height(node->right.get()) : 0;
    }
    
    void update_height(Node* node) {
        if (node) {
            node->height = 1 + std::max(height(node->left.get()), height(node->right.get()));
        }
    }
    
    // Rotations
    std::unique_ptr<Node> rotate_right(std::unique_ptr<Node> y) {
        auto x = std::move(y->left);
        y->left = std::move(x->right);
        update_height(y.get());
        x->right = std::move(y);
        update_height(x.get());
        return x;
    }
    
    std::unique_ptr<Node> rotate_left(std::unique_ptr<Node> x) {
        auto y = std::move(x->right);
        x->right = std::move(y->left);
        update_height(x.get());
        y->left = std::move(x);
        update_height(y.get());
        return y;
    }
    
    // Balance the tree
    std::unique_ptr<Node> balance(std::unique_ptr<Node> node) {
        if (!node) return nullptr;
        
        update_height(node.get());
        int bf = balance_factor(node.get());
        
        // Left heavy
        if (bf > 1) {
            if (balance_factor(node->left.get()) < 0) {
                // Left-Right case
                node->left = rotate_left(std::move(node->left));
            }
            // Left-Left case
            return rotate_right(std::move(node));
        }
        
        // Right heavy
        if (bf < -1) {
            if (balance_factor(node->right.get()) > 0) {
                // Right-Left case
                node->right = rotate_right(std::move(node->right));
            }
            // Right-Right case
            return rotate_left(std::move(node));
        }
        
        return node;
    }
    
    // Insert a node
    std::unique_ptr<Node> insert_node(std::unique_ptr<Node> node, const K& key, const V& value) {
        if (!node) {
            size_++;
            return std::make_unique<Node>(key, value);
        }
        
        if (key < node->key) {
            node->left = insert_node(std::move(node->left), key, value);
        } else if (key > node->key) {
            node->right = insert_node(std::move(node->right), key, value);
        } else {
            // Update value if key already exists
            node->value = value;
            return node;
        }
        
        return balance(std::move(node));
    }
    
    // Find the node with minimum key
    Node* find_min(Node* node) const {
        return node->left ? find_min(node->left.get()) : node;
    }
    
    // Remove a node
    std::unique_ptr<Node> remove_node(std::unique_ptr<Node> node, const K& key) {
        if (!node) return nullptr;
        
        if (key < node->key) {
            node->left = remove_node(std::move(node->left), key);
        } else if (key > node->key) {
            node->right = remove_node(std::move(node->right), key);
        } else {
            // Node to be deleted found
            
            // Case 1: Leaf node
            if (!node->left && !node->right) {
                size_--;
                return nullptr;
            }
            
            // Case 2: Node with only one child
            if (!node->left) {
                size_--;
                return std::move(node->right);
            }
            if (!node->right) {
                size_--;
                return std::move(node->left);
            }
            
            // Case 3: Node with two children
            // Find inorder successor (smallest in right subtree)
            Node* successor = find_min(node->right.get());
            
            // Copy successor's data to this node
            node->key = successor->key;
            node->value = successor->value;
            
            // Delete the successor
            node->right = remove_node(std::move(node->right), successor->key);
        }
        
        return balance(std::move(node));
    }
    
    // Find a node
    const Node* find_node(const Node* node, const K& key) const {
        if (!node) return nullptr;
        
        if (key < node->key) {
            return find_node(node->left.get(), key);
        } else if (key > node->key) {
            return find_node(node->right.get(), key);
        } else {
            return node;
        }
    }
    
    // In-order traversal
    void inorder_traversal(const Node* node, std::vector<std::pair<K, V>>& result) const {
        if (!node) return;
        
        inorder_traversal(node->left.get(), result);
        result.emplace_back(node->key, node->value);
        inorder_traversal(node->right.get(), result);
    }
    
    // Range query
    void range_query(const Node* node, const K& min_key, const K& max_key, 
                    std::vector<std::pair<K, V>>& result) const {
        if (!node) return;
        
        if (node->key > min_key) {
            range_query(node->left.get(), min_key, max_key, result);
        }
        
        if (node->key >= min_key && node->key <= max_key) {
            result.emplace_back(node->key, node->value);
        }
        
        if (node->key < max_key) {
            range_query(node->right.get(), min_key, max_key, result);
        }
    }
    
public:
    AVLTree() : root_(nullptr), size_(0) {}
    
    // Core operations
    void insert(const K& key, const V& value) {
        root_ = insert_node(std::move(root_), key, value);
    }
    
    bool remove(const K& key) {
        size_t old_size = size_;
        root_ = remove_node(std::move(root_), key);
        return size_ < old_size;
    }
    
    std::optional<V> find(const K& key) const {
        const Node* node = find_node(root_.get(), key);
        if (node) {
            return node->value;
        }
        return std::nullopt;
    }
    
    // Utility functions
    size_t size() const {
        return size_;
    }
    
    bool empty() const {
        return size_ == 0;
    }
    
    void clear() {
        root_ = nullptr;
        size_ = 0;
    }
    
    // Range operations
    std::vector<std::pair<K, V>> range(const K& min_key, const K& max_key) const {
        std::vector<std::pair<K, V>> result;
        range_query(root_.get(), min_key, max_key, result);
        return result;
    }
    
    // Get all elements in sorted order
    std::vector<std::pair<K, V>> get_all() const {
        std::vector<std::pair<K, V>> result;
        inorder_traversal(root_.get(), result);
        return result;
    }
};

} // namespace my_redis

#endif // AVL_TREE_H 