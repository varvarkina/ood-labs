#ifndef WALTZBEHAVIOR_H
#define WALTZBEHAVIOR_H

#include "IDanceBehavior.h"
#include <iostream>

class WaltzBehavior : public IDanceBehavior
{
public:
	void Dance() const override
	{
		std::cout << "I'm dancing waltz!" << std::endl;
	}
};

#endif
