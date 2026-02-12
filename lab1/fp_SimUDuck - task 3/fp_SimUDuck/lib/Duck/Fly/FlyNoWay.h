#ifndef FLYNOWAY_H
#define FLYNOWAY_H

#include "IFlyBehavior.h"

FlyBehavior MakeFlyNoWay()
{
	return
	{
		[]() {},
		[]() { return 0; },
		[]() { return false; }
	};
}

#endif