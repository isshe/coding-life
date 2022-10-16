
#include "List.h"
#include "ListIterator.h"
#include "Iterator.h"
#include "Student.h"

int main(void) {

    Student std1 = Student(1);
    Student std2 = Student(2);
    Student std3 = Student(3);

    List<Student> list(10);

    list.Append(std1);
    list.Append(std2);
    list.Append(std3);

    ListIterator<Student> stdIter(&list);

    for (stdIter.First(); !stdIter.IsDone(); stdIter.Next()) {
        stdIter.CurrentItem().print();
    }

    return 0;
}