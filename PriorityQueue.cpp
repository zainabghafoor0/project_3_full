#include "PriorityQueue.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>

PriorityQueue::PriorityQueue() {}

bool PriorityQueue::compare(const std::pair<std::string, size_t>& a,
                            const std::pair<std::string, size_t>& b) {
    // Sort by count descending (higher counts first)
    if (a.second != b.second) {
        return a.second > b.second;
    }
    // For ties, sort by word ascending (lexicographically)
    return a.first < b.first;
}

void PriorityQueue::buildQueue(const std::vector<std::pair<std::string, size_t>>& frequencies) {
    data_ = frequencies;
    std::sort(data_.begin(), data_.end(), compare);
}

error_type PriorityQueue::writeToFile(const std::string& filename) const {
    std::ofstream out(filename, std::ios::out | std::ios::trunc);
    if (!out.is_open()) {
        return UNABLE_TO_OPEN_FILE_FOR_WRITING;
    }
    
    for (const auto& item : data_) {
        out << item.first << " " << item.second << '\n';
        if (!out) {
            std::cerr << "Error: failed while writing to " << filename << "\n";
            return FAILED_TO_WRITE_FILE;
        }
    }
    
    return NO_ERROR;
}

size_t PriorityQueue::size() const {
    return data_.size();
}

bool PriorityQueue::isEmpty() const {
    return data_.empty();
}

void PriorityQueue::insert(const std::pair<std::string, size_t>& item) {
    data_.push_back(item);
    std::sort(data_.begin(), data_.end(), compare);
}

std::pair<std::string, size_t> PriorityQueue::extractMin() {
    if (data_.empty()) {
        return {"", 0};
    }
    // Since we sort in descending order, "min" priority is at the back
    auto item = data_.back();
    data_.pop_back();
    return item;
}

std::pair<std::string, size_t> PriorityQueue::findMin() const {
    if (data_.empty()) {
        return {"", 0};
    }
    return data_.back();
}