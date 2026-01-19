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

