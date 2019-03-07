#ifndef _STUDENT_H_
#define _STUDENT_H_

#include "Student.h"
#include <iostream>

Student::Student(long id) {
    _id = id;
}

void Student::print() {
    std::cout << "---Student::Say()---" << _id << "---" << std::endl;
}

#endif