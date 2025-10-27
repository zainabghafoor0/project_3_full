//
// Created by Ali Kooshesh on 9/27/25.
//

#include "Scanner.hpp"
#include <utility>
#include <iostream>
#include <fstream>
#include "utils.hpp"

Scanner::Scanner(std::filesystem::path inputPath) 
    : inputPath_(std::move(inputPath)) {
}

error_type Scanner::tokenize(std::vector<std::string>& words) {
    std::ifstream inFile(inputPath_);
    if (!inFile.is_open()) {
        return UNABLE_TO_OPEN_FILE;
    }
    
    std::string word;
    while (!(word = readWord(inFile)).empty()) {
        words.push_back(word);
    }
    
    inFile.close();
    return NO_ERROR;
}

error_type Scanner::tokenize(std::vector<std::string>& words,
                            const std::filesystem::path& outputFile) {
    // First, tokenize into memory using the other overload
    error_type status = tokenize(words);
    if (status != NO_ERROR) {
        return status;
    }
    
    // Then write to the output file
    return writeVectorToFile(outputFile.string(), words);
}

std::string Scanner::readWord(std::istream& in) {
    std::string word;
    char c;
    
    // Skip everything until we find a letter (a-z or A-Z)
    while (in.get(c)) {
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
            // Convert to lowercase and start the word
            if (c >= 'A' && c <= 'Z') {
                word += (c - 'A' + 'a');
            } else {
                word += c;
            }
            break;
        }
    }
    
    // If no letter was found, we've reached end of input
    if (word.empty()) {
        return "";
    }
    
    // Continue building the word with letters and internal apostrophes
    while (in.get(c)) {
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
            // Letter - add it (converting to lowercase if needed)
            if (c >= 'A' && c <= 'Z') {
                word += (c - 'A' + 'a');
            } else {
                word += c;
            }
        } else if (c == '\'') {
            // Apostrophe - only include if followed by a letter (internal apostrophe)
            char next = in.peek();
            if ((next >= 'a' && next <= 'z') || (next >= 'A' && next <= 'Z')) {
                // It's internal (followed by a letter) - include it
                word += '\'';
            } else {
                // Not followed by a letter - end the word
                break;
            }
        } else {
            // Any other character is a separator - end the word
            break;
        }
    }
    
    return word;
}