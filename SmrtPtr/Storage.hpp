#ifndef STORAGE_HPP
#define STORAGE_HPP

#include "DynamicArray.hpp"
#include <cstddef>

struct Record {
    void* object;
    size_t count;
    size_t size;
    void (*deleter)(void*);

    Record(void* object, void (*deleter)(void*)) : object(object), count(1), size(1), deleter(deleter) {}
    Record(void* object, size_t size, void (*deleter)(void*)) : object(object), count(1), size(size), deleter(deleter) {}
};

template <typename T>
void DeleteObject(void* object) {
    delete static_cast<T*>(object);
}

template <typename T>
void DeleteArray(void* array) {
    delete[] static_cast<T *>(array);
}

class Storage {
private:
    DynamicArray<Record*> records;

    size_t Find(Record* record) {
        size_t i = 0;
        while (i < records.GetSize() && records[i] != record) {
            i++;
        }
        return i;
    }

public:
    static Storage GetStorage() {
        static Storage storage;
        return storage;
    }

    template <typename T>
    Record* CreateObject(const T& item) {
        T *object = new T(item);
        Record *newRecord = new Record(object, DeleteObject<T>);
        records.Add(newRecord);
        return newRecord;
    }

    template <typename T>
    Record* CreateArray(size_t size) {
        T *arr = new T[size];
        Record *newRecord = new Record(arr, size, DeleteArray<T>);
        records.Add(newRecord);
        return newRecord;
    }

    void Destroy(Record* record) {
        size_t index = Find(record);
        if (index < records.GetSize()) {
            record->deleter(record->object);
            delete records[index];
            records.RemoveAt(index);
        }
    }
};

#endif