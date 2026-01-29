# Giveaway Winner Selector
# Usage:
#   make        - Build the program
#   make run    - Build and run the program
#   make clean  - Remove compiled binary
#
# Interactive commands when running:
#   y - Confirm winner as verified
#   n - Remove entry and pick another winner
#   p - Print winner's full JSON data

CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall
TARGET = choose

all: $(TARGET)

$(TARGET): choose.cpp json.hpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) choose.cpp
	chmod +x $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all run clean

