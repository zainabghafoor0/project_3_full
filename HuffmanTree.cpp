#include "HuffmanTree.hpp"
#include <algorithm>
#include <map>
#include <iostream>

// ============================================================================
// NEW FILE FOR PHASE 3: HuffmanTree implementation
// ============================================================================

HuffmanTree::HuffmanTree() : root_(nullptr) {}

HuffmanTree::~HuffmanTree() {
    destroy(root_);
}

void HuffmanTree::destroy(TreeNode* node) {
    if (node == nullptr) return;
    destroy(node->left);
    destroy(node->right);
    delete node;
}

void HuffmanTree::buildFromFrequencies(const std::vector<std::pair<std::string, size_t>>& freqs) {
    if (freqs.empty()) {
        root_ = nullptr;
        return;
    }
    
    // Special case: single word
    if (freqs.size() == 1) {
        root_ = new TreeNode(freqs[0].first, freqs[0].second);
        return;
    }
    
    // Create a working copy sorted by frequency (smallest first, then lexicographically)
    // The input is sorted descending, so we reverse it
    std::vector<TreeNode*> nodes;
    for (auto it = freqs.rbegin(); it != freqs.rend(); ++it) {
        nodes.push_back(new TreeNode(it->first, it->second));
    }
    
    // Build Huffman tree using greedy algorithm
    while (nodes.size() > 1) {
        // Take two nodes with smallest frequency (at the beginning)
        TreeNode* left = nodes[0];
        TreeNode* right = nodes[1];
        nodes.erase(nodes.begin(), nodes.begin() + 2);
        
        // Create parent node
        TreeNode* parent = new TreeNode(left, right);
        
        // Insert parent back maintaining sorted order
        // Find insertion point to keep sorted by frequency (ascending)
        auto insertPos = std::lower_bound(nodes.begin(), nodes.end(), parent,
            [](const TreeNode* a, const TreeNode* b) {
                if (a->frequency != b->frequency) {
                    return a->frequency < b->frequency;
                }
                // For tie-breaking, prefer the node that was created earlier
                // (this gives deterministic behavior)
                return false;
            });
        nodes.insert(insertPos, parent);
    }
    
    root_ = nodes[0];
}

int HuffmanTree::getHeight() const {
    return height(root_);
}

int HuffmanTree::height(TreeNode* node) const {
    if (node == nullptr) return 0;
    if (node->isLeaf()) return 1;
    
    int leftHeight = height(node->left);
    int rightHeight = height(node->right);
    
    return 1 + std::max(leftHeight, rightHeight);
}

void HuffmanTree::assignCodes(std::vector<std::pair<std::string, std::string>>& out) const {
    out.clear();
    if (root_ == nullptr) return;
    
    // Special case: single word gets code "0"
    if (root_->isLeaf()) {
        out.push_back({root_->word, "0"});
        return;
    }
    
    assignCodesDFS(root_, "", out);
}

void HuffmanTree::assignCodesDFS(TreeNode* node, const std::string& code,
                                  std::vector<std::pair<std::string, std::string>>& out) const {
    if (node == nullptr) return;
    
    if (node->isLeaf()) {
        out.push_back({node->word, code});
        return;
    }
    
    // Traverse left (add '0') before right (add '1')
    if (node->left) assignCodesDFS(node->left, code + "0", out);
    if (node->right) assignCodesDFS(node->right, code + "1", out);
}

error_type HuffmanTree::writeHeader(std::ostream& os) const {
    if (!os.good()) {
        return UNABLE_TO_OPEN_FILE_FOR_WRITING;
    }
    
    if (root_ == nullptr) return NO_ERROR;
    
    // Special case: single word
    if (root_->isLeaf()) {
        os << root_->word << " 0\n";
        if (!os) return FAILED_TO_WRITE_FILE;
        return NO_ERROR;
    }
    
    // Write header in pre-order
    writeHeaderPreorder(root_, "", os);
    
    if (!os) return FAILED_TO_WRITE_FILE;
    return NO_ERROR;
}

void HuffmanTree::writeHeaderPreorder(TreeNode* node, const std::string& code, std::ostream& os) const {
    if (node == nullptr) return;
    
    if (node->isLeaf()) {
        os << node->word << " " << code << '\n';
        return;
    }
    
    // Visit left before right (pre-order)
    if (node->left) writeHeaderPreorder(node->left, code + "0", os);
    if (node->right) writeHeaderPreorder(node->right, code + "1", os);
}

error_type HuffmanTree::encode(const std::vector<std::string>& tokens,
                                std::ostream& os_bits,
                                int wrap_cols) const {
    if (!os_bits.good()) {
        return UNABLE_TO_OPEN_FILE_FOR_WRITING;
    }
    
    if (root_ == nullptr || tokens.empty()) {
        os_bits << '\n';
        return NO_ERROR;
    }
    
    // Build codebook
    std::vector<std::pair<std::string, std::string>> codeList;
    assignCodes(codeList);
    
    std::map<std::string, std::string> codebook;
    for (const auto& pair : codeList) {
        codebook[pair.first] = pair.second;
    }
    
    // Encode tokens
    int col = 0;
    for (const auto& token : tokens) {
        auto it = codebook.find(token);
        if (it == codebook.end()) {
            std::cerr << "Error: Token '" << token << "' not found in codebook\n";
            return FAILED_TO_WRITE_FILE;
        }
        
        const std::string& code = it->second;
        for (char bit : code) {
            os_bits << bit;
            col++;
            
            if (col >= wrap_cols) {
                os_bits << '\n';
                col = 0;
            }
        }
    }
    
    // Write final newline if we haven't just written one
    if (col > 0) {
        os_bits << '\n';
    }
    
    if (!os_bits) return FAILED_TO_WRITE_FILE;
    return NO_ERROR;
}

bool HuffmanTree::isEmpty() const {
    return root_ == nullptr;
}
