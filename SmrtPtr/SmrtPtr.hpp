#ifndef SMRTPTR_HPP
#define SMRTPTR_HPP

#include "Storage.hpp"
#include <cstddef>
#include <type_traits>

template <typename T>
class SmrtPtr {
private:
    template <typename>
    friend class SmrtPtr;

    Record* record;

    void Release() {
        if (record == nullptr) {
            return;
        }
        record->count--;
        if (record->count == 0) {
            Storage::GetStorage().Destroy(record);
        }
        record = nullptr;
    }

public:
    SmrtPtr(const T& value) : record(Storage::GetStorage().CreateObject(value)) {}

    SmrtPtr(const SmrtPtr<T>& other) : record(other.record) {
        if (other.record != nullptr) {
            record->count++;
        }
    }

    template <typename U, typename = std::enable_if_t<std::is_convertible_v<U*, T*>>>
    SmrtPtr(const SmrtPtr<U>& other) : record(other.record) {
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
        if (record == nullptr)
            throw NullPointerException("Null pointer");
        return *static_cast<T*>(record->object);
    }

    T* operator->() {
        if (record == nullptr)
            throw NullPointerException("Null pointer");
        return static_cast<T*>(record->object);
    }
};

template <typename T>
class SmrtPtr<T[]> {
private:
    Record *record;

    void Release() {
        if (record == nullptr) {
            return;
        }
        record->count--;
        if (record->count == 0) {
            Storage::GetStorage().Destroy(record);
        }
        record = nullptr;
    }

public:
    SmrtPtr(size_t size) : record(Storage::GetStorage().CreateArray<T>(size)) {}

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
        if (record == nullptr)
            throw NullPointerException("Null pointer");
        if (index >= record->size)
            throw IndexOutOfRange(index, record->size);
        return static_cast<T*>(record->object)[index];
    }

};

#endif