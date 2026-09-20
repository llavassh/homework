#include <cassert>
#include "SmrtPtr.hpp"

void TestSmrtPtr() {
    SmrtPtr<int> p1(42);
    assert(*p1 == 42);
    auto p2 = p1;
    assert(*p2 == 42);
    *p1 = 100;
    assert(*p1 == 100);
    assert(*p2 == 100);
}

void TestSmrtPtrArray() {
    SmrtPtr<int[]> arr(10);
    arr[0] = 42;
    arr[9] = 100;
    assert(arr[0] == 42);
    assert(arr[9] == 100);

    bool thrown1 = false;
    try {
        arr[10];
    } catch(IndexOutOfRange&) {
        thrown1 = true;
    }
    assert(thrown1);
}

int main() {
    TestSmrtPtr();
    TestSmrtPtrArray();
    return 0;
}