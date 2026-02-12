#ifndef DECOYDUCK_H
#define DECOYDUCK_H

#include "Duck.h"
#include "Fly/FlyNoWay.h"
#include "Quack/MuteQuackBehavior.h"
#include "Dance/DisableDanceBehavior.h"
#include <iostream>
#include <memory>

class DecoyDuck : public Duck
{
public:
	DecoyDuck()
		: Duck(MakeFlyNoWay(), MakeMuteQuackBehavior(), MakeDisableDanceBehavior())
	{
	}

	void Display() const override
	{
		std::cout << "I'm decoy duck" << std::endl;
	}
};

#endif
