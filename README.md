# Project 3: Huffman Trees and Coding

**Name:** Zainab Ghafoor  
**ID:** 008259427  
**GitHub:** https://github.com/zainabghafoor0/project_3_full.git

---

## Overview

Word-level Huffman encoding implementation using Binary Search Trees for frequency counting and greedy algorithm for optimal prefix-free code generation.

**Implementation:**
- Phase 1: Tokenization (Scanner)
- Phase 2: Frequency counting (BST + PriorityQueue)
- Phase 3: Huffman encoding (HuffmanTree)

---

## Files

```
project_3_full/
├── main.cpp              # Main program
├── Scanner.cpp/hpp       # Tokenization
├── BST.cpp/hpp           # Frequency counting
├── PriorityQueue.cpp/hpp # Sorted frequency list
├── HuffmanTree.cpp/hpp   # Huffman tree (Phase 3)
├── utils.cpp/hpp         # Utilities
├── Makefile              # Build file
└── input_output/         # I/O directory
```

---

## Compilation

**Using Makefile:**
```bash
cp /home/faculty/kooshesh/cs315_fall2025/project3/final_version/copy_files.bash .
cp /home/faculty/kooshesh/cs315_fall2025/project3/final_version/compile_and_test_project3_final_version.bash
bash ./copy_files.bash
bash ./compile_and_test_project3_final_version.bash

g++ -std=c++20 *.cpp -Wall -o p3_complete.x
./p3_complete.x input_output/TheBells.txt
```
---

## Usage

```bash
./huffman_encoder input_output/TheBells.txt
```

**Output Files:**
- `TheBells.tokens` - Tokenized words
- `TheBells.freq` - Word frequencies
- `TheBells.hdr` - Codebook (word→code)
- `TheBells.code` - Encoded bitstream

**Example Output:**
```
BST height: 4
BST unique words: 5
Total tokens: 8
Min frequency: 1
Max frequency: 2
Huffman tree height: 3
Total letters in words: 28
Total encoded bits: 18
```

---

## Output Format

**TheBells.hdr** (codebook):
```
cat 000
dog 001
fast 01
ran 10
the 11
```

**TheBells.code** (encoded, 80 chars/line):
```
1100110110011011000001100110110011011
```
