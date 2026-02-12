#ifndef REDHEADDUCK_H
#define REDHEADDUCK_H

#include "Duck.h"
#include "Fly/FlyWithWings.h"
#include "Quack/QuackBehavior.h"
#include "Dance/MinuetBehavior.h"

#include <memory>

class RedheadDuck : public Duck
{
public:
	RedheadDuck()
		: Duck(MakeFlyWithWings(), MakeQuackBehavior(), MakeMinuetBehavior())
	{
	}

	void Display() const override
	{
		std::cout << "I'm redhead duck" << std::endl;
	}
};

#endif