#ifndef FLYNOWAY_H
#define FLYNOWAY_H

#include "IFlyBehavior.h"

class FlyNoWay : public IFlyBehavior
{
public:
	void Fly() override {}

	int GetFlightCounter() const override
	{
		return 0;
	}

	bool CanFly() const override
	{
		return false;
	}
};

#endif