#ifndef DYNAMICARRAY_HPP
#define DYNAMICARRAY_HPP

#include "Exceptions.hpp"
#include <cstddef>

template <typename T>
class DynamicArray {
private:
    T* data;
    size_t size;
    size_t capacity;

public:
    DynamicArray(): data(nullptr), size(0), capacity(0) {}

    explicit DynamicArray(size_t size): size(size), capacity(size) {
        if (size < 0) 
            throw std::invalid_argument("Negative size");
        data = (size > 0) ? new T[size] : nullptr; 
    }

    DynamicArray(T* items, size_t count): size(count), capacity(count) {
        if (count > 0) {
            data = new T[count];
            for (size_t i = 0; i < count; i++) {
                data[i] = items[i];
            }
        }
        else {
            data = nullptr;
        }
    }

    DynamicArray(const DynamicArray<T> &other): size(other.size), capacity(other.capacity) {
        if (capacity > 0) {
            data = new T[capacity];
            for (size_t i = 0; i < size; i++) {
                data[i] = other.data[i];
            }
        }
        else {
            data = nullptr;
        }
    }

    ~DynamicArray() {
        delete[] data;
    }

    DynamicArray<T>& operator=(const DynamicArray<T>& other) {
        if (this != &other) {
            delete[] data;
            size = other.size;
            capacity = other.capacity;
            if (capacity > 0) {
                data = new T[capacity];
                for (size_t i = 0; i < size; i++) {
                    data[i] = other.data[i];
                }
            }
            else {
                data = nullptr;
            }
        }
        return *this;
    }

    const T& Get(size_t index) const {
        if (index >= size) throw IndexOutOfRange(index, size);
        
        return data[index];
    }

    void Set(size_t index, const T &value) {
        if (index < 0 || index >= size)
            throw IndexOutOfRange(index, size);

        data[index] = value;
    }

    size_t GetSize() const {
        return size;
    }

    size_t GetCapacity() const {
        return capacity;
    }
  
    void Resize(size_t newSize) {
        if (newSize > capacity) {
            size_t newCapacity = (capacity == 0) ? 1 : capacity;
            while (newSize > newCapacity) {
                newCapacity *= 2;
            }
            T *newData = new T[newCapacity];
            for (size_t i = 0; i < size; i++) {
                newData[i] = data[i];
            }
            delete[] data;
            data = newData;
            capacity = newCapacity;
        }
        size = newSize;
    }

    void Add(const T &item) {
        Resize(size + 1);
        data[size - 1] = item;
    }

    void RemoveAt(size_t index) {
        if (index >= size)
            throw IndexOutOfRange(index, size);
        for (size_t i = index; i < size - 1; i++) {
            data[i] = data[i + 1];
        }
        size--;
    }

    T &operator[](size_t index) {
        if (index >= size) throw IndexOutOfRange(index, size);
        return data[index];
    }

    const T &operator[](size_t index) const {
        if (index < 0 || index >= size) throw IndexOutOfRange(index, size);
        return data[index];
    }

};
#endif