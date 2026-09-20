#ifndef SMRTPTR_HPP
#define SMRTPTR_HPP

#include "Storage.hpp"
#include <cstddef>

template <typename T>
class SmrtPtr {
private:
    inline static Storage<T> storage;
    Record<T>* record;

    void Release() {
        if (record == nullptr) {
            return;
        }
        record->count--;
        if (record->count == 0) {
            storage.DestroyObject(record);
        }
    }

public:
    SmrtPtr(const T& value) : record(storage.CreateObject(value)) {}

    SmrtPtr(const SmrtPtr& other) : record(other.record) {
        if (other.record != nullptr) {
            record->count++;
        }
    }

    SmrtPtr(SmrtPtr&& other) : record(other.record) {
        other.record = nullptr;
    }

    ~SmrtPtr() {
        Release();
    }

    SmrtPtr<T>& operator=(const SmrtPtr& other) {
        if (this == &other) {
            return *this;
        }
        Release();
        record = other.record;
        if (record != nullptr) {
            record->count++;
        }
        return *this;
    }

    SmrtPtr<T>& operator=(SmrtPtr&& other) noexcept {
        if (this == &other) {
            return *this;
        }
        Release();
        record = other.record;
        other.record = nullptr;
        return *this;
    }

    T& operator*() {
        return *(record->object);
    }

    T* operator->() {
        return record->object;
    }
};

template <typename T>
class SmrtPtr<T[]> {
private:
    inline static Storage<T> storage;
    Record<T> *record;

    void Release() {
        if (record == nullptr) {
            return;
        }
        record->count--;
        if (record->count == 0) {
            storage.DestroyArray(record);
        }
    }

public:
    SmrtPtr(size_t size) : record(storage.CreateArray(size)) {}

    SmrtPtr(const SmrtPtr<T[]>& other) : record(other.record) {
        if (other.record != nullptr) {
            record->count++;
        }
    }

    SmrtPtr(SmrtPtr<T[]>&& other) : record(other.record) {
        other.record = nullptr;
    }

    ~SmrtPtr() {
        Release();
    }

    SmrtPtr<T[]>& operator=(const SmrtPtr<T[]>& other) {
        if (this == &other) {
            return *this;
        }
        Release();
        record = other.record;
        if (record != nullptr) {
            record->count++;
        }
        return *this;
    }

    SmrtPtr<T[]>& operator=(SmrtPtr<T[]>&& other) noexcept {
        if (this == &other) {
            return *this;
        }
        Release();
        record = other.record;
        other.record = nullptr;
        return *this;
    }

    T& operator[](size_t index) {
        if (index >= record->size)
            throw IndexOutOfRange(index, record->size);
        return record->object[index];
    }

};

#endif