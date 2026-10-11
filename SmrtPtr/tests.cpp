#include <cassert>
#include <iostream>
#include <type_traits>
#include "SmrtPtr.hpp"
#include "Person.hpp"

int passed = 0;
int failed = 0;

#define CHECK(condition) \
    do { \
        if (condition) { \
            passed++; \
        } else { \
            failed++; \
                assert(false); \
        } \
    } while (false)

void TestSmrtPtr() {
    SmrtPtr<int> p1(42);
    CHECK(*p1 == 42);
    SmrtPtr<int> p2 = p1;
    CHECK(*p2 == 42);
    *p1 = 100;
    CHECK(*p1 == 100);
    CHECK(*p2 == 100);
}

void TestSmrtPtrAssignment() {
    SmrtPtr<int> p1(10);
    SmrtPtr<int> p2(20);
    p2 = p1;
    CHECK(*p1 == 10);
    CHECK(*p2 == 10);
    *p1 = 50;
    CHECK(*p2 == 50);
}

void TestSmrtPtrMove() {
    SmrtPtr<int> p1(42);
    SmrtPtr<int> p2 = std::move(p1);
    CHECK(*p2 == 42);
}

void TestSmrtPtrSelfAssignment() {
    SmrtPtr<int> p1(42);
    p1 = p1;
    CHECK(*p1 == 42);
}

void TestSmrtPtrArray() {
    SmrtPtr<int[]> arr(10);
    arr[0] = 42;
    arr[5] = 77;
    arr[9] = 100;
    CHECK(arr[0] == 42);
    CHECK(arr[5] == 77);
    CHECK(arr[9] == 100);
}

void TestSmrtPtrArrayCopy() {
    SmrtPtr<int[]> arr1(5);
    arr1[0] = 10;
    arr1[1] = 20;
    SmrtPtr<int[]> arr2 = arr1;
    CHECK(arr2[0] == 10);
    CHECK(arr2[1] == 20);
    arr1[0] = 100;
    CHECK(arr2[0] == 100);
}

void TestSmrtPtrArrayAssignment() {
    SmrtPtr<int[]> arr1(5);
    SmrtPtr<int[]> arr2(5);
    arr1[0] = 10;
    arr2[0] = 20;
    arr2 = arr1;
    CHECK(arr2[0] == 10);
    arr1[0] = 50;
    CHECK(arr2[0] == 50);
}

void TestSmrtPtrArrayMove() {
    SmrtPtr<int[]> arr1(5);
    arr1[0] = 42;
    SmrtPtr<int[]> arr2 = std::move(arr1);
    CHECK(arr2[0] == 42);
}

void TestSmrtPtrArrayBounds() {
    SmrtPtr<int[]> arr(5);
    arr[0] = 10;
    arr[4] = 20;
    CHECK(arr[0] == 10);
    CHECK(arr[4] == 20);
    bool thrown = false;
    try {
        arr[5];
    }
    catch (const IndexOutOfRange&) {
        thrown = true;
    }
    CHECK(thrown);
}

void TestSmrtPtrSubTypes() {
    static_assert(std::is_constructible_v<SmrtPtr<Person>, SmrtPtr<Student>>);
    static_assert(!std::is_constructible_v<SmrtPtr<Person>, SmrtPtr<int>>);
    SmrtPtr<Student> stud = Student(20, 1);
    SmrtPtr<Person> pers = stud;
    CHECK(pers->age == 20);

    SmrtPtr<Student> stud1 = Student(19, 2);
    {
        SmrtPtr<Person> pers1 = stud1;
        pers1->age = 21;
        CHECK(pers1->age == 21);
        CHECK(stud1->age == 21);
    }
    CHECK(stud1->age == 21);
}

int main() {
    TestSmrtPtr();
    TestSmrtPtrAssignment();
    TestSmrtPtrMove();
    TestSmrtPtrSelfAssignment();

    TestSmrtPtrArray();
    TestSmrtPtrArrayCopy();
    TestSmrtPtrArrayAssignment();
    TestSmrtPtrArrayMove();
    TestSmrtPtrArrayBounds();

    TestSmrtPtrSubTypes();

    std::cout << "Passed tests: " << passed << std::endl;
    std::cout << "Failed tests: " << failed << std::endl;

    return 0;
}