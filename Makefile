# Makefile for Phase 3 - Huffman Encoder
# Compile with: make
# Clean with: make clean

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic
TARGET = huffman_encoder

# Source files
SOURCES = main.cpp \
          Scanner.cpp \
          BST.cpp \
          PriorityQueue.cpp \
          HuffmanTree.cpp \
          utils.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Header files for dependency tracking
HEADERS = Scanner.hpp \
          BST.hpp \
          PriorityQueue.hpp \
          HuffmanTree.hpp \
          utils.hpp

# Default target
all: $(TARGET)

# Link object files to create executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)
	@echo "Build complete! Run with: ./$(TARGET) input_output/yourfile.txt"

# Compile source files to object files
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJECTS) $(TARGET)
	@echo "Clean complete!"

# Create input_output directory
setup:
	mkdir -p input_output
	@echo "Created input_output directory"

# Run a test (requires test.txt in input_output/)
test: $(TARGET)
	./$(TARGET) input_output/test.txt

.PHONY: all clean setup test
