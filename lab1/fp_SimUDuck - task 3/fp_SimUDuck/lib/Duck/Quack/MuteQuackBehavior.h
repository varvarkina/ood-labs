#ifndef MUTEQUAKBEHAVIOR_H
#define MUTEQUAKBEHAVIOR_H

#include "IQuackBehavior.h"

QuackBehaviorStrategy MakeMuteQuackBehavior()
{
	return { []() {} };
};

#endif
