#ifndef STORAGE_HPP
#define STORAGE_HPP

#include "DynamicArray.hpp"
#include <cstddef>

template <typename T>
struct Record {
    T* object;
    size_t count;
    size_t size;

    Record(T* object) : object(object), count(1), size(1) {}
    Record(T* object, size_t size) : object(object), count(1), size(size) {}
};

template <typename T>
class Storage {
private:
    DynamicArray<Record<T> *> records;

    size_t Find(Record<T>* record) {
        size_t i = 0;
        while (i < records.GetSize() && records[i] != record) {
            i++;
        }
        return i;
    }

public:
    Record<T>* CreateObject(const T& item) {
        T *object = new T(item);
        Record<T> *newRecord = new Record<T>(object);
        records.Add(newRecord);
        return newRecord;
    }

    Record<T>* CreateArray(size_t size) {
        T *arr = new T[size];
        Record<T> *newRecord = new Record<T>(arr, size);
        records.Add(newRecord);
        return newRecord;
    }

    void DestroyObject(Record<T>* record) {
        size_t index = Find(record);
        if (index < records.GetSize()) {
            delete record->object;
            delete records[index];
            records.RemoveAt(index);
        }
    }

    void DestroyArray(Record<T>* record) {
        size_t index = Find(record);
        if (index < records.GetSize()) {
            delete[] record->object;
            delete records[index];
            records.RemoveAt(index);
        }
    }
};

#endif