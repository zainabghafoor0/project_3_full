#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <map>                    // *** NEW FOR PHASE 3: For codebook map ***

#include "Scanner.hpp"
#include "BST.hpp"
#include "PriorityQueue.hpp"
#include "HuffmanTree.hpp"        // *** NEW FOR PHASE 3: Include Huffman tree ***
#include "utils.hpp"

int main(int argc, char *argv[]) {
    // 1) Parse and validate arguments
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }

    const std::string dirName = std::string("input_output");
    const std::string inputFileName = std::string(argv[1]);
    const std::string inputFileBaseName = baseNameWithoutTxt(inputFileName);

    // Build paths for output files
    const std::string wordTokensFileName = dirName + "/" + inputFileBaseName + ".tokens";
    const std::string freqFileName = dirName + "/" + inputFileBaseName + ".freq";
    const std::string hdrFileName = dirName + "/" + inputFileBaseName + ".hdr";      // *** NEW FOR PHASE 3 ***
    const std::string codeFileName = dirName + "/" + inputFileBaseName + ".code";    // *** NEW FOR PHASE 3 ***

    // Verify input file, directory exist and output files are writable
    if (error_type status; (status = regularFileExistsAndIsAvailable(inputFileName)) != NO_ERROR)
        exitOnError(status, inputFileName);

    if (error_type status; (status = directoryExists(dirName)) != NO_ERROR)
        exitOnError(status, dirName);

    if (error_type status; (status = canOpenForWriting(wordTokensFileName)) != NO_ERROR)
        exitOnError(status, wordTokensFileName);

    if (error_type status; (status = canOpenForWriting(freqFileName)) != NO_ERROR)
        exitOnError(status, freqFileName);

    // *** NEW FOR PHASE 3: Verify new output files are writable ***
    if (error_type status; (status = canOpenForWriting(hdrFileName)) != NO_ERROR)
        exitOnError(status, hdrFileName);

    if (error_type status; (status = canOpenForWriting(codeFileName)) != NO_ERROR)
        exitOnError(status, codeFileName);
    // *** END NEW FOR PHASE 3 ***

    // 2) Scanner: tokenize input file
    std::vector<std::string> words;
    auto fileToWords = Scanner(std::filesystem::path(inputFileName));
    
    if (error_type status; (status = fileToWords.tokenize(words)) != NO_ERROR)
        exitOnError(status, inputFileName);

    // Write tokens to .tokens file
    if (error_type status; (status = writeVectorToFile(wordTokensFileName, words)) != NO_ERROR)
        exitOnError(status, wordTokensFileName);

    // 3) BST: build tree from tokens and compute frequencies
    BST bst;
    bst.buildFromTokens(words);
    
    // Get frequency data
    auto frequencies = bst.getFrequencies();
    
    // 4) Print BST measures to stdout
    size_t totalTokens = words.size();
    size_t uniqueWords = bst.getUniqueWords();
    int bstHeight = bst.getHeight();
    size_t minFreq, maxFreq;
    bst.getMinMaxFrequency(minFreq, maxFreq);
    
    std::cout << "BST height: " << bstHeight << '\n';
    std::cout << "BST unique words: " << uniqueWords << '\n';
    std::cout << "Total tokens: " << totalTokens << '\n';
    std::cout << "Min frequency: " << minFreq << '\n';
    std::cout << "Max frequency: " << maxFreq << '\n';

    // 5) PriorityQueue: sort by count (desc) then word (asc), write to .freq
    PriorityQueue pq;
    pq.buildQueue(frequencies);
    
    if (error_type status; (status = pq.writeToFile(freqFileName)) != NO_ERROR)
        exitOnError(status, freqFileName);

    // ============================================================================
    // *** NEW FOR PHASE 3: Build Huffman Tree and Encode ***
    // ============================================================================
    
    // 6) Build Huffman tree from frequencies
    HuffmanTree huffman;
    huffman.buildFromFrequencies(frequencies);
    
    // Print Huffman tree height
    int huffmanHeight = huffman.getHeight();
    std::cout << "Huffman tree height: " << huffmanHeight << '\n';
    
    // 7) Write header file (.hdr) - codebook with word->code mappings
    std::ofstream hdrFile(hdrFileName);
    if (!hdrFile.is_open()) {
        exitOnError(UNABLE_TO_OPEN_FILE_FOR_WRITING, hdrFileName);
    }
    
    if (error_type status; (status = huffman.writeHeader(hdrFile)) != NO_ERROR) {
        exitOnError(status, hdrFileName);
    }
    hdrFile.close();
    
    // 8) Encode tokens and write to .code file
    std::ofstream codeFile(codeFileName);
    if (!codeFile.is_open()) {
        exitOnError(UNABLE_TO_OPEN_FILE_FOR_WRITING, codeFileName);
    }
    
    if (error_type status; (status = huffman.encode(words, codeFile, 80)) != NO_ERROR) {
        exitOnError(status, codeFileName);
    }
    codeFile.close();
    
    // 9) Calculate and print additional statistics
    // Calculate total letters in all words
    size_t totalLetters = 0;
    for (const auto& word : words) {
        totalLetters += word.length();
    }
    std::cout << "Total letters in words: " << totalLetters << '\n';
    
    // Calculate total encoded bits
    std::vector<std::pair<std::string, std::string>> codebook;
    huffman.assignCodes(codebook);
    
    // Build a map for quick lookup
    std::map<std::string, std::string> codeMap;
    for (const auto& pair : codebook) {
        codeMap[pair.first] = pair.second;
    }
    
    size_t totalBits = 0;
    for (const auto& word : words) {
        auto it = codeMap.find(word);
        if (it != codeMap.end()) {
            totalBits += it->second.length();
        }
    }
    std::cout << "Total encoded bits: " << totalBits << '\n';
    
    // *** END NEW FOR PHASE 3 ***
    // ============================================================================

    // 10) Success
    return 0;
}