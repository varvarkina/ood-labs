#ifndef MALLARDDUCK_H
#define MALLARDDUCK_H

#include "Duck.h"
#include "Fly/FlyWithWings.h"
#include "Quack/QuackBehavior.h"
#include "Dance/WaltzBehavior.h"

class MallardDuck : public Duck
{
public:
	MallardDuck()
		: Duck(MakeFlyWithWings(), MakeQuackBehavior(), MakeWaltzBehavior())
	{
	}

	void Display() const override
	{
		std::cout << "I'm mallard duck" << std::endl;
	}
};

#endif