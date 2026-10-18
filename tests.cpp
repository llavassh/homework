#include "UnqPtr.hpp"
#include "ShrdPtr.hpp"
#include "Sequence.hpp"
#include "Person.hpp"
#include <cassert>
#include <iostream>
#include <utility>

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

void TestUnqPtr() {
    UnqPtr<int> a(42);
    CHECK(*a == 42);
    UnqPtr<int> b = std::move(a);
    CHECK(*b == 42);
    bool thrown = false;
    try {
        *a;
    } catch(NullPointerException&) {
        thrown = true;
    }
    CHECK(thrown);
}

void TestUnqPtrAssignment() {
    UnqPtr<int> a(42);
    UnqPtr<int> b(100);
    a = std::move(b);
    CHECK(*a == 100);
    bool thrown = false;
    try {
        *b;
    } catch(NullPointerException&) {
        thrown = true;
    }
    CHECK(thrown);
    a = std::move(a);
    CHECK(*a == 100);
}

void TestUnqPtrSubTypes() {
    static_assert(std::is_constructible_v<UnqPtr<Person>, UnqPtr<Student>>);
    static_assert(!std::is_constructible_v<UnqPtr<Person>, UnqPtr<int>>);
    UnqPtr<Student> stud = Student(20, 1);
    UnqPtr<Person> pers = std::move(stud);
    CHECK(pers->age == 20);
}

void TestUnqPtrArray() {
    UnqPtr<int[]> arr(3);
    arr[0] = 10;
    arr[1] = 20;
    arr[2] = 30;
    CHECK(arr[0] == 10);
    CHECK(arr[1] == 20);
    CHECK(arr[2] == 30);
    bool thrown = false;
    try {
        arr[5];
    }
    catch (IndexOutOfRange&) {
        thrown = true;
    }
    CHECK(thrown);
}

void TestUnqPtrArrayMove() {
    UnqPtr<int[]> a(3);
    a[0] = 10;
    a[1] = 20;
    a[2] = 30;
    UnqPtr<int[]> b = std::move(a);
    CHECK(b[0] == 10);
    CHECK(b[1] == 20);
    CHECK(b[2] == 30);
    bool thrown = false;
    try {
        a[0];
    }
    catch (NullPointerException&) {
        thrown = true;
    }
    CHECK(thrown);
}

void TestUnqPtrArrayAssignment() {
    UnqPtr<int[]> a(3);
    UnqPtr<int[]> b(2);
    b[0] = 10;
    b[1] = 20;
    a = std::move(b);
    CHECK(a[0] == 10);
    CHECK(a[1] == 20);
}

void TestShrdPtr() {
    UnqPtr<int> a(42);
    ShrdPtr<int> b(a);
    CHECK(*a == 42);
    CHECK(*b == 42);
    ShrdPtr<int> c = std::move(b);
    bool thrown = false;
    try {
        *b;
    } catch(NullPointerException&) {
        thrown = true;
    }
    CHECK(thrown);
}

void TestShrdPtrCopy() {
    UnqPtr<int> a(42);
    ShrdPtr<int> b(a);
    ShrdPtr<int> c(b);
    CHECK(*b == 42);
    CHECK(*c == 42);
}

void TestShrdPtrMultiple() {
    UnqPtr<int> a(42);
    ShrdPtr<int> b(a);
    ShrdPtr<int> c(a);
    CHECK(*b == 42);
    CHECK(*c == 42);
}

void TestShrdPtrAssignment() {
    UnqPtr<int> a(42);
    UnqPtr<int> b(100);
    ShrdPtr<int> x(a);
    ShrdPtr<int> y(b);
    x = y;
    CHECK(*x == 100);
    CHECK(*y == 100);
}

void TestShrdPtrMoveAssignment() {
    UnqPtr<int> a(42);
    UnqPtr<int> b(100);
    ShrdPtr<int> x(a);
    ShrdPtr<int> y(b);
    x = std::move(y);
    CHECK(*x == 100);
    bool thrown = false;
    try {
        *y;
    } catch (NullPointerException&) {
        thrown = true;
    }
    CHECK(thrown);
    x = std::move(x);
    CHECK(*x == 100);
}

void TestShrdPtrMovedFromDestruction() {
    UnqPtr<int> a(42);

    {
        ShrdPtr<int> b(a);
        ShrdPtr<int> c = std::move(b);
    }

    CHECK(*a == 42);
}

ShrdPtr<Person> CreatePersonPtr(UnqPtr<Student>& stud) {
    ShrdPtr<Student> studentPtr(stud);
    ShrdPtr<Person> personPtr(studentPtr);
    return personPtr;
}

void TestShrdPtrSubTypes() {
    static_assert(std::is_constructible_v<ShrdPtr<Person>, ShrdPtr<Student>>);
    static_assert(!std::is_constructible_v<ShrdPtr<Person>, ShrdPtr<int>>);
    UnqPtr<Student> stud(Student(20, 1));
    ShrdPtr<Student> studentPtr(stud);
    ShrdPtr<Person> personPtr(studentPtr);
    CHECK(personPtr->age == 20);

    UnqPtr<Student> stud2(Student(19, 2));
    ShrdPtr<Person> personPtr2 = CreatePersonPtr(stud2);
    CHECK(personPtr2->age == 19);
}

void TestShrdPtrArray() {
    UnqPtr<int[]> owner(3);
    ShrdPtr<int[]> ptr(owner);
    ptr[0] = 10;
    ptr[1] = 20;
    ptr[2] = 30;
    CHECK(ptr[0] == 10);
    CHECK(ptr[1] == 20);
    CHECK(ptr[2] == 30);
}

void TestShrdPtrArrayCopyConstructor() {
    UnqPtr<int[]> owner(3);
    ShrdPtr<int[]> first(owner);
    first[0] = 10;
    first[1] = 20;
    first[2] = 30;
    ShrdPtr<int[]> second(first);
    CHECK(second[0] == 10);
    CHECK(second[1] == 20);
    CHECK(second[2] == 30);
    second[1] = 99;
    CHECK(first[1] == 99);
}

void TestShrdPtrArrayCopyAssignment() {
    UnqPtr<int[]> owner(3);
    UnqPtr<int[]> anotherOwner(2);
    ShrdPtr<int[]> first(owner);
    ShrdPtr<int[]> second(anotherOwner);
    first[0] = 10;
    first[1] = 20;
    first[2] = 30;
    second[0] = 100;
    second[1] = 200;
    first = second;
    CHECK(first[0] == 100);
    CHECK(first[1] == 200);
    bool thrown = false;
    try {
        first[2];
    } catch(IndexOutOfRange&) {
        thrown = true;
    }
    CHECK(thrown);
}

void TestShrdPtrArrayMoveConstructor() {
    UnqPtr<int[]> owner(3);
    ShrdPtr<int[]> first(owner);
    first[0] = 10;
    first[1] = 20;
    first[2] = 30;
    ShrdPtr<int[]> second(std::move(first));
    CHECK(second[0] == 10);
    CHECK(second[1] == 20);
    CHECK(second[2] == 30);
    bool thrown = false;
    try {
        first[0];
    }
    catch (const NullPointerException&) {
        thrown = true;
    }
    CHECK(thrown);
}

void TestShrdPtrArrayMoveAssignment() {
    UnqPtr<int[]> owner(3);
    UnqPtr<int[]> anotherOwner(2);
    ShrdPtr<int[]> first(owner);
    ShrdPtr<int[]> second(anotherOwner);
    first[0] = 10;
    first[1] = 20;
    first[2] = 30; 
    second[0] = 100;
    second[1] = 200;
    first = std::move(second);
    CHECK(first[0] == 100);
    CHECK(first[1] == 200);
    bool thrown = false;
    try {
        first[2];
    } catch(IndexOutOfRange&) {
        thrown = true;
    }
    CHECK(thrown);
}

void TestSequenceWithUnqPtr() {
    Sequence<int> seq;
    CHECK(seq.GetLength() == 0);
    CHECK(seq.GetLength() == 0);
    seq.Add(10);
    seq.Add(20);
    seq.Add(30);
    CHECK(seq.GetLength() == 3);
    CHECK(seq[0] == 10);
    CHECK(seq[1] == 20);
    CHECK(seq[2] == 30);
    CHECK(seq.GetCapacity() == 4);
    seq.Add(40);
    seq.Add(50);
    CHECK(seq[3] == 40);
    CHECK(seq[4] == 50);
    CHECK(seq.GetLength() == 5);
    CHECK(seq.GetCapacity() == 8);
    bool thrown = false;
    try {
        seq[100];
    } catch(IndexOutOfRange&) {
        thrown = true;
    }
    CHECK(thrown);
}

int main() {
    TestUnqPtr();
    TestUnqPtrAssignment();
    TestUnqPtrSubTypes();
    TestUnqPtrArray();
    TestUnqPtrArrayMove();
    TestUnqPtrArrayAssignment();

    TestShrdPtr();
    TestShrdPtrCopy();
    TestShrdPtrMultiple();
    TestShrdPtrAssignment();
    TestShrdPtrMoveAssignment();
    TestShrdPtrMovedFromDestruction();
    TestShrdPtrSubTypes();
    TestShrdPtrArray();
    TestShrdPtrArrayCopyConstructor();
    TestShrdPtrArrayCopyAssignment();
    TestShrdPtrArrayMoveConstructor();
    TestShrdPtrArrayMoveAssignment();

    TestSequenceWithUnqPtr();

    std::cout << "Passed tests: " << passed << std::endl;
    std::cout << "Failed tests: " << failed << std::endl;

    return 0;
}