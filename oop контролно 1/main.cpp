#include <iostream>
#include "Subject.h"
#include "Plan.h"

int main()
{
	Plan plan(320);

	plan.printSub();

	Subject sub1("OOP", "Anotaciq nqkakva", 60, false, Grade::Empty);

	plan += sub1;

	plan.gradeExam("OOP", Grade::Good);

	plan.printSub();

	Subject sub2("LA", "Lineyna algebra", 56, false, Grade::Empty);

	plan += sub2;

	plan.printSub();

	plan.gradeExam("LA", Grade::Average);

	plan.printSub();

	Subject sub3("DS", "Diskretni strukturi pri Minko", 78, false, Grade::Empty);

	plan += sub3;

	plan.printSub();

	std::cout << plan.getCurrentCredits() << std::endl;

	std::cout << plan.getAverage() << std::endl;

	plan -= sub3;

	plan.printSub();

	return 0;
}