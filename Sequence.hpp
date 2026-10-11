#include "UnqPtr.hpp"
#include <cstddef>

template <typename T>
class Sequence {
private:
    UnqPtr<T[]> data;
    size_t size;
    size_t capacity;

public:
    Sequence() : data(0), size(0), capacity(0) {}

    void Add(const T& item) {
        if (size == capacity) {
            size_t newCapacity = (capacity == 0) ? 1 : capacity * 2;
            capacity = newCapacity;
            UnqPtr<T[]> newData(capacity);
            for (size_t i = 0; i < size; i++) {
                newData[i] = data[i];
            }
            data = std::move(newData);
        }
        data[size] = item;
        size++;
    }

    size_t GetLength() {
        return size;
    }

    size_t GetCapacity() {
        return capacity;
    }

    T& operator[](size_t index) {
        if (index >= size)
            throw IndexOutOfRange(index, size);
        return data[index];
    }

};