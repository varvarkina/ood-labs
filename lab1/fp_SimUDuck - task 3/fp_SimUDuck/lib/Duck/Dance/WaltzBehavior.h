#ifndef WALTZBEHAVIOR_H
#define WALTZBEHAVIOR_H

#include "IDanceBehavior.h"
#include <iostream>

DanceBehavior MakeWaltzBehavior()
{
	return { []() {
		   std::cout << "I'm dancing waltz!" << std::endl;
	   } };
}

#endif
