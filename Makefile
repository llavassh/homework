CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
SANITIZE_FLAGS = -fsanitize=address,undefined -g

run:
	$(CXX) $(CXXFLAGS)  tests.cpp -o tests 
	tests

memory-check: tests
	drmemory -ignore_kernel --
tests.exe

clean: 
	del tests.exe

.PHONY: test memory-check clean