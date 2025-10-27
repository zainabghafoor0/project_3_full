#include "BST.hpp"
#include <algorithm>
#include <limits>

BST::BST() : root_(nullptr) {}

BST::~BST() {
    destroy(root_);
}

void BST::destroy(Node* node) {
    if (node == nullptr) return;
    destroy(node->left);
    destroy(node->right);
    delete node;
}

void BST::insert(const std::string& word) {
    insert(root_, word);
}

void BST::insert(Node*& node, const std::string& word) {
    if (node == nullptr) {
        node = new Node(word);
        return;
    }
    
    if (word == node->word) {
        // Word already exists, increment count
        node->count++;
    } else if (word < node->word) {
        insert(node->left, word);
    } else {
        insert(node->right, word);
    }
}

void BST::buildFromTokens(const std::vector<std::string>& tokens) {
    for (const auto& token : tokens) {
        insert(token);
    }
}

int BST::getHeight() const {
    return height(root_);
}

int BST::height(Node* node) const {
    if (node == nullptr) return 0;
    
    int leftHeight = height(node->left);
    int rightHeight = height(node->right);
    
    return 1 + std::max(leftHeight, rightHeight);
}

size_t BST::getUniqueWords() const {
    if (root_ == nullptr) return 0;
    
    std::vector<std::pair<std::string, size_t>> frequencies;
    inorderTraversal(root_, frequencies);
    return frequencies.size();
}

std::vector<std::pair<std::string, size_t>> BST::getFrequencies() const {
    std::vector<std::pair<std::string, size_t>> result;
    inorderTraversal(root_, result);
    return result;
}

void BST::inorderTraversal(Node* node, std::vector<std::pair<std::string, size_t>>& result) const {
    if (node == nullptr) return;
    
    inorderTraversal(node->left, result);
    result.push_back({node->word, node->count});
    inorderTraversal(node->right, result);
}

void BST::getMinMaxFrequency(size_t& minFreq, size_t& maxFreq) const {
    if (root_ == nullptr) {
        minFreq = 0;
        maxFreq = 0;
        return;
    }
    
    minFreq = std::numeric_limits<size_t>::max();
    maxFreq = 0;
    getMinMax(root_, minFreq, maxFreq);
}

void BST::getMinMax(Node* node, size_t& minFreq, size_t& maxFreq) const {
    if (node == nullptr) return;
    
    minFreq = std::min(minFreq, node->count);
    maxFreq = std::max(maxFreq, node->count);
    
    getMinMax(node->left, minFreq, maxFreq);
    getMinMax(node->right, minFreq, maxFreq);
}

bool BST::isEmpty() const {
    return root_ == nullptr;
}