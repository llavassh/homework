#include <cstring>

class Person {
public:
    virtual ~Person() = default;
    int age;
    Person(int age) : age(age) {}
};

class Student : public Person {
public:
    int group;
    Student(int age, int group) : Person(age), group(group) {}
};