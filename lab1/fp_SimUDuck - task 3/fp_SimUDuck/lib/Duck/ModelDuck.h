#ifndef MODELDUCK_H
#define MODELDUCK_H

#include "Duck.h"
#include "Fly/FlyNoWay.h"
#include "Quack/QuackBehavior.h"
#include "Dance/DisableDanceBehavior.h"

class ModelDuck : public Duck
{
public:
	ModelDuck()
		: Duck(MakeFlyNoWay(), MakeQuackBehavior(), MakeDisableDanceBehavior())
	{
	}

	void Display() const override
	{
		std::cout << "I'm model duck" << std::endl;
	}
};

#endif