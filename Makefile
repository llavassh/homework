CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
DRMEMORY_CXXFLAGS = -std=c++17 -Wall -Wextra -g -O0 -fno-inline -fno-omit-frame-pointer -static-libgcc -static-libstdc++

SANITIZER_CXX = "C:/Program Files/LLVM/bin/clang++.exe"
SANITIZER_CXXFLAGS = -std=c++17 -Wall -Wextra -g -O1 -fsanitize=address,undefined -fno-omit-frame-pointer

sanitize:
	$(SANITIZER_CXX) $(SANITIZER_CXXFLAGS) tests.cpp -o tests_sanitized.exe
	cmd /c "set PATH=C:\Program Files\LLVM\lib\clang\20\lib\windows;%PATH% && tests_sanitized.exe"

memory-check:
	g++ $(DRMEMORY_CXXFLAGS) tests.cpp -o tests_drmemory.exe
	drmemory -leaks_only -- tests_drmemory.exe

run:
	$(CXX) $(CXXFLAGS) tests.cpp -o tests.exe
	tests.exe

benchmark:
	$(CXX) $(CXXFLAGS) benchmark.cpp -o benchmark.exe
	benchmark.exe

clean:
	del tests.exe tests_sanitized.exe tests_drmemory.exe benchmark.exe

.PHONY: run benchmark sanitize memory-check clean