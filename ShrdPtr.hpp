#include "UnqPtr.hpp"
#include <cstddef>

template <typename T>
class ShrdPtr {
private:
    template <typename>
    friend class ShrdPtr;

    UnqPtrBase *owner;
    size_t *count;

    void Release() {
        if (owner != nullptr) {
            (*count)--;
            if (*count == 0) {
                owner->ResetCount();
                delete count;
                count = nullptr;
            }
        } 
    }

public:
    ShrdPtr(UnqPtr<T>& owner) : owner(&owner), count(owner.count) {
        if (count == nullptr) {
            count = new size_t(1);
            owner.count = count;
        }
        else {
            (*count)++;
        }
    }

    ShrdPtr(const ShrdPtr<T>& other) : owner(other.owner), count(other.count) {
        if (owner != nullptr)
            (*count)++;
    }

    template <typename U, typename = std::enable_if_t<std::is_convertible_v<U*, T*>>>
    ShrdPtr(const ShrdPtr<U>& other) : owner(other.owner), count(other.count) {
        if (owner != nullptr)
            (*count)++;
    }

    ShrdPtr(ShrdPtr&& other) noexcept : owner(other.owner), count(other.count) {
        other.owner = nullptr;
        other.count = nullptr;
    }

    ~ShrdPtr() {
        Release();
    }

    ShrdPtr<T>& operator=(const ShrdPtr<T>& other) {
        if (this != &other) {
            Release();
            owner = other.owner;
            count = other.count;
            if (owner != nullptr) 
                (*count)++;
        }
        return *this;
    }

    ShrdPtr<T>& operator=(ShrdPtr<T>&& other) {
        if (this != &other) {
            Release();
            owner = other.owner;
            count = other.count;
            other.owner = nullptr;
            other.count = nullptr;
        }
        return *this;
    }

    const T& operator*() {
        if (owner == nullptr)
            throw NullPointerException("Null pointer");
        return *(static_cast<T*>(owner->Raw()));
    }

    T* operator->() {
        if (owner == nullptr)
            throw NullPointerException("Null pointer");
        return static_cast<T*>(owner->Raw());
    }
};

#include "UnqPtr.hpp"
#include <cstddef>

template <typename T>
class ShrdPtr<T[]> {
private:
    template <typename>
    friend class ShrdPtr;

    UnqPtrBase *owner;
    size_t *count;

    void Release() {
        if (owner != nullptr) {
            (*count)--;
            if (*count == 0) {
                owner->ResetCount();
                delete count;
                count = nullptr;
            }
        } 
    }

public:
    ShrdPtr(UnqPtr<T[]>& owner) : owner(&owner), count(owner.count) {
        if (count == nullptr) {
            count = new size_t(1);
            owner.count = count;
        }
        else {
            (*count)++;
        }
    }

    ShrdPtr(const ShrdPtr<T[]>& other) : owner(other.owner), count(other.count) {
        if (owner != nullptr)
            (*count)++;
    }

    ShrdPtr(ShrdPtr<T[]>&& other) noexcept : owner(other.owner), count(other.count) {
        other.owner = nullptr;
        other.count = nullptr;
    }

    ~ShrdPtr() {
        Release();
    }

    ShrdPtr<T[]>& operator=(const ShrdPtr<T[]>& other) {
        if (this != &other) {
            Release();
            owner = other.owner;
            count = other.count;
            if (owner != nullptr) 
                (*count)++;
        }
        return *this;
    }

    ShrdPtr<T[]>& operator=(ShrdPtr<T[]>&& other) {
        if (this != &other) {
            Release();
            owner = other.owner;
            count = other.count;
            other.owner = nullptr;
            other.count = nullptr;
        }
        return *this;
    }

    T& operator[](size_t index) {
        if (owner == nullptr)
            throw NullPointerException("Null pointer");
        if (index >= owner->GetSize())
            throw IndexOutOfRange(index, owner->GetSize());
        return static_cast<T*>(owner->Raw())[index];
    }

};