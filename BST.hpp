#ifndef BST_HPP
#define BST_HPP

#include <string>
#include <vector>
#include <utility>

class BST {
private:
    struct Node {
        std::string word;
        size_t count;
        Node* left;
        Node* right;
        
        Node(const std::string& w) 
            : word(w), count(1), left(nullptr), right(nullptr) {}
    };
    
    Node* root_;
    
    // Helper functions
    void insert(Node*& node, const std::string& word);
    void destroy(Node* node);
    int height(Node* node) const;
    void inorderTraversal(Node* node, std::vector<std::pair<std::string, size_t>>& result) const;
    void getMinMax(Node* node, size_t& minFreq, size_t& maxFreq) const;
    
public:
    BST();
    ~BST();
    
    // Insert a word (or increment count if it exists)
    void insert(const std::string& word);
    
    // Build BST from a vector of tokens
    void buildFromTokens(const std::vector<std::string>& tokens);
    
    // Get tree height (0 for empty tree)
    int getHeight() const;
    
    // Get number of unique words
    size_t getUniqueWords() const;
    
    // Get all (word, count) pairs in lexicographic order
    std::vector<std::pair<std::string, size_t>> getFrequencies() const;
    
    // Get min and max frequencies
    void getMinMaxFrequency(size_t& minFreq, size_t& maxFreq) const;
    
    // Check if tree is empty
    bool isEmpty() const;
};

#endif // BST_HPP