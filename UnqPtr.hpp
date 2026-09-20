#ifndef UNQPTR_HPP
#define UNQPTR_HPP

template <typename T>
class UnqPtr {
private:
    T *ptr;

public:
    UnqPtr(T *ptr) : ptr(ptr) {}
    UnqPtr(UnqPtr &other) = delete;
};

#endif