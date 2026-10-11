#ifndef UNQPTR_HPP
#define UNQPTR_HPP

#include "Exceptions.hpp"
#include <type_traits>

class UnqPtrBase {
public: 
    virtual ~UnqPtrBase() = default;

    virtual void *Raw() const noexcept = 0;
};

template <typename T>
class ShrdPtr;

template <typename T>
class UnqPtr : public UnqPtrBase {
private:
    template <typename>
    friend class UnqPtr;

    template <typename>
    friend class ShrdPtr;

    T *ptr;
    size_t *count;

public:
    UnqPtr(T item) : ptr(new T(item)), count(nullptr) {}

    ~UnqPtr() {
        delete ptr;
    }

    UnqPtr(const UnqPtr<T> &other) = delete;

    UnqPtr(UnqPtr<T>&& other) noexcept : ptr(other.ptr), count(other.count) {
        other.ptr = nullptr;
        other.count = nullptr;
    }

    template <typename U, typename = std::enable_if_t<std::is_convertible_v<U*, T*>>>
    UnqPtr(UnqPtr<U>&& other) noexcept : ptr(other.ptr), count(other.count) {
        other.ptr = nullptr;
        other.count = nullptr;
    }

    UnqPtr &operator=(const UnqPtr &other) = delete;

    UnqPtr &operator=(UnqPtr&& other) noexcept {
        if (this == &other) {
            return *this;
        }
        delete ptr;
        ptr = other.ptr;
        other.ptr = nullptr;
        count = other.count;
        other.count = nullptr;
        return *this;
    }

    void* Raw() const noexcept override {
        return ptr;
    }

    T& operator*() {
        if (ptr == nullptr)
            throw NullPointerException("Null pointer");
        return *ptr;
    }

    T* operator->() {
        if (ptr == nullptr)
            throw NullPointerException("Null pointer");
        return ptr;
    }
};

template <typename T>
class UnqPtr<T[]> : public UnqPtrBase {
private:
    template <typename>
    friend class UnqPtr;

    template <typename>
    friend class ShrdPtr;

    T *data;
    size_t *count;
    size_t size;

public:
    UnqPtr(size_t size) : data(new T[size]), count(nullptr), size(size) {}

    ~UnqPtr() {
        delete[] data;
    }

    UnqPtr(const UnqPtr<T[]> &other) = delete;

    UnqPtr(UnqPtr<T[]>&& other) noexcept : data(other.data), count(other.count), size(other.size) {
        other.data = nullptr;
        other.count = nullptr;
        other.size = 0;
    }

    UnqPtr &operator=(const UnqPtr &other) = delete;

    UnqPtr &operator=(UnqPtr&& other) noexcept {
        if (this == &other) {
            return *this;
        }
        delete[] data;
        data = other.data;
        size = other.size;
        count = other.count;
        other.data = nullptr;
        other.size = 0;
        other.count = nullptr;
        return *this;
    }

    void* Raw() const noexcept override {
        return data;
    }

    T& operator[](size_t index) {
        if (data == nullptr)
            throw NullPointerException("Null pointer");
        if (index >= size)
            throw IndexOutOfRange(index, size);
        return data[index];
    }
};

#endif