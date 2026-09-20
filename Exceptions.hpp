#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP
 
#include <string>
#include <stdexcept>

class IndexOutOfRange: public std::out_of_range {
public:
    IndexOutOfRange(size_t index, size_t size) : std::out_of_range("Index " + std::to_string(index) + 
    " out of range (size: " + std::to_string(size) + ")") {}
};

class EmptyContainer: public std::logic_error {
public:
    explicit EmptyContainer(const std::string &name)
    :std::logic_error(name + " is empty") {}
};

class NullPointerException: public std::runtime_error {
public:
    NullPointerException(const std::string &) : std::runtime_error("Null pointer") {}
};

#endif