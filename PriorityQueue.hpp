#ifndef PRIORITYQUEUE_HPP
#define PRIORITYQUEUE_HPP

#include <vector>
#include <string>
#include <utility>
#include "utils.hpp"

class PriorityQueue {
private:
    std::vector<std::pair<std::string, size_t>> data_;
    
    // Comparator: sort by count descending, then word ascending (for ties)
    static bool compare(const std::pair<std::string, size_t>& a,
                       const std::pair<std::string, size_t>& b);
    
public:
    PriorityQueue();
    
    // Build queue from frequency vector and sort it
    void buildQueue(const std::vector<std::pair<std::string, size_t>>& frequencies);
    
    // Write sorted frequencies to file
    error_type writeToFile(const std::string& filename) const;
    
    // Get the current size
    size_t size() const;
    
    // Check if empty
    bool isEmpty() const;
    
    // Additional interface methods (for potential testing)
    void insert(const std::pair<std::string, size_t>& item);
    std::pair<std::string, size_t> extractMin();
    std::pair<std::string, size_t> findMin() const;
};

#endif // PRIORITYQUEUE_HPP