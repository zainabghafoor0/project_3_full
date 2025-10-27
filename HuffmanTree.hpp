#ifndef HUFFMANTREE_HPP
#define HUFFMANTREE_HPP

#include <string>
#include <vector>
#include <utility>
#include <ostream>
#include "utils.hpp"

// ============================================================================
// NEW FILE FOR PHASE 3: HuffmanTree class
// ============================================================================

class HuffmanTree {
public:
    HuffmanTree();
    ~HuffmanTree();
    
    // Build Huffman tree from frequency list
    // Input: vector of (word, count) pairs sorted by count descending
    void buildFromFrequencies(const std::vector<std::pair<std::string, size_t>>& freqs);
    
    // Get the height of the Huffman tree
    int getHeight() const;
    
    // Generate codebook by traversing tree (left=0, right=1)
    // Output: vector of (word, bitstring_code) pairs
    void assignCodes(std::vector<std::pair<std::string, std::string>>& out) const;
    
    // Write header file: "word code\n" for each leaf in pre-order
    error_type writeHeader(std::ostream& os) const;
    
    // Encode tokens using the Huffman tree
    // Writes ASCII '0'/'1' wrapped at wrap_cols characters per line
    error_type encode(const std::vector<std::string>& tokens,
                      std::ostream& os_bits,
                      int wrap_cols = 80) const;
    
    // Check if tree is empty
    bool isEmpty() const;

private:
    struct TreeNode {
        std::string word;      // empty for internal nodes
        size_t frequency;      // combined frequency for internal nodes
        TreeNode* left;
        TreeNode* right;
        
        // Constructor for leaf node
        TreeNode(const std::string& w, size_t freq)
            : word(w), frequency(freq), left(nullptr), right(nullptr) {}
        
        // Constructor for internal node
        TreeNode(TreeNode* l, TreeNode* r)
            : word(""), frequency(l->frequency + r->frequency),
              left(l), right(r) {}
        
        bool isLeaf() const { return left == nullptr && right == nullptr; }
    };
    
    TreeNode* root_;
    
    // Helper functions
    void destroy(TreeNode* node);
    int height(TreeNode* node) const;
    void assignCodesDFS(TreeNode* node, const std::string& code,
                       std::vector<std::pair<std::string, std::string>>& out) const;
    void writeHeaderPreorder(TreeNode* node, const std::string& code, std::ostream& os) const;
};

#endif // HUFFMANTREE_HPP
