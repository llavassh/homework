#include "Sequence.hpp"
#include "DynamicArray.hpp"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <fstream>

long long BenchmarkRawPointersContainer(size_t count) {
    auto start = std::chrono::steady_clock::now();
    {
        DynamicArray<int> array;
        for (size_t i = 0; i < count; i++) {
            array.Add(i);
        }
    }
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    return duration.count();
}

long long BenchmarkUnqPointersContainer(size_t count) {
    auto start = std::chrono::steady_clock::now(); 
    {
        Sequence<int> seq;
        for (size_t i = 0; i < count; i++) {
            seq.Add(i);
        }
    }   
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    return duration.count();
} 

void Sort(long long* array, size_t size) {
    for (size_t i = 0; i < size; i++) {
        for (size_t j = size - i - 1; j > 0; j--) {
            if (array[j] < array[j - 1]) {
                long long temp = array[j];
                array[j] = array[j - 1];
                array[j - 1] = temp;
            }
        }
    }
}

long long BenchmarkRawPointers(size_t count) {
    long long sum = 0;
    auto start = std::chrono::steady_clock::now();
    for (size_t i = 0; i < count; i++) {
        int *ptr = new int(i);
        int value = *ptr;
        sum += value;
        delete ptr;
    }
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    if (sum == -1) {
        std::cerr << sum;
    }
    return duration.count();
}

long long BenchmarkUnqPointers(size_t count) {
    long long sum = 0;
    auto start = std::chrono::steady_clock::now();
    for (size_t i = 0; i < count; i++) { 
        UnqPtr<int> ptr(i);
        int value = *ptr;
        sum += value;
    }
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    if (sum == -1) {
        std::cerr << sum;
    }
    return duration.count();
}

int main() {
    std::ofstream file("benchmark.txt");
    size_t counts[] = {10, 100, 1000, 10000, 100000, 1000000, 10000000};

    long long ptrTimes[7];
    long long unqPtrTimes[7];
    for (size_t i = 0; i < 7; i++) {
        long long ptrTimeOne[21];
        long long unqPtrTimeOne[21];
        for (size_t j = 0; j < 21; j++) {
            if (j % 2 == 0) {
                ptrTimeOne[j] = BenchmarkRawPointers(counts[i]);
                unqPtrTimeOne[j] = BenchmarkUnqPointers(counts[i]);
            } else {
                unqPtrTimeOne[j] = BenchmarkUnqPointers(counts[i]);
                ptrTimeOne[j] = BenchmarkRawPointers(counts[i]);
            }
        }

        Sort(ptrTimeOne, 21);
        ptrTimes[i] = ptrTimeOne[10];
        
        Sort(unqPtrTimeOne, 21);
        unqPtrTimes[i] = unqPtrTimeOne[10];
    }

    long long ptrTimesContainer[7];
    long long unqPtrTimesContainer[7];
    for (size_t i = 0; i < 7; i++) {
        long long ptrTimeOneContainer[21];
        long long unqPtrTimeOneContainer[21];
        for (size_t j = 0; j < 21; j++) {
            if (j % 2 == 0) {
                ptrTimeOneContainer[j] = BenchmarkRawPointersContainer(counts[i]);
                unqPtrTimeOneContainer[j] = BenchmarkUnqPointersContainer(counts[i]);
            } else {
                unqPtrTimeOneContainer[j] = BenchmarkUnqPointersContainer(counts[i]);
                ptrTimeOneContainer[j] = BenchmarkRawPointersContainer(counts[i]);
            }
        }
        Sort(ptrTimeOneContainer, 21);
        ptrTimesContainer[i] = ptrTimeOneContainer[10];
        
        Sort(unqPtrTimeOneContainer, 21);
        unqPtrTimesContainer[i] = unqPtrTimeOneContainer[10];
    }

    file << "Benchmark results: \n";

    file << '+' << std::string(20, '-') << '+' << std::string(25, '-') << '+' << std::string(25, '-') << '+' << std::endl;
    file << '|' << std::left << std::setw(20) << "Elements count" << "|"
         << std::left << std::setw(25) << "Raw pointers" << "|"
         << std::left << std::setw(25) << "My unique pointers" << '|' << std::endl;
    file << '+' << std::string(20, '-') << '+' << std::string(25, '-') << '+' << std::string(25, '-') << '+' << std::endl;
    for (size_t i = 0; i < 7; i++) {
        file << '|' << std::right << std::setw(20) << counts[i] << '|'
             << std::right << std::setw(25) << ptrTimes[i] << '|'
             << std::right << std::setw(25) << unqPtrTimes[i] << '|' << std::endl;
    }
    file << '+' << std::string(20, '-') << '+' << std::string(25, '-') << '+' << std::string(25, '-') << '+' << std::endl;

    file << '+' << std::string(20, '-') << '+' << std::string(25, '-') << '+' << std::string(25, '-') << '+' << std::endl;
    file << '|' << std::left << std::setw(20) << "Elements count" << "|"
         << std::left << std::setw(25) << "Raw pointers(container)" << "|"
         << std::left << std::setw(25) << "My unique pointers(container)" << '|' << std::endl;
    file << '+' << std::string(20, '-') << '+' << std::string(25, '-') << '+' << std::string(25, '-') << '+' << std::endl;
    for (size_t i = 0; i < 7; i++) {
        file << '|' << std::right << std::setw(20) << counts[i] << '|'
             << std::right << std::setw(25) << ptrTimesContainer[i] << '|'
             << std::right << std::setw(25) << unqPtrTimesContainer[i] << '|' << std::endl;
    }
    file << '+' << std::string(20, '-') << '+' << std::string(25, '-') << '+' << std::string(25, '-') << '+' << std::endl;
    return 0;
}