CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude

all: build/termchrone build/x2j

build/termchrone: src/main.cpp
	mkdir -p build
	$(CXX) $(CXXFLAGS) $< -o $@

build/x2j: x2j
	mkdir -p build
	cp x2j $@

clean:
	rm -f build/termchrone build/x2j

run: all
	./build/termchrone