#ifndef MINUETBEHAVIOR_H
#define MINUETBEHAVIOR_H

#include "IDanceBehavior.h"
#include <iostream>

DanceBehavior MakeMinuetBehavior()
{
	return { []() {
		std::cout << "I'm dancing minuet!" << std::endl;
	} };
}

#endif
