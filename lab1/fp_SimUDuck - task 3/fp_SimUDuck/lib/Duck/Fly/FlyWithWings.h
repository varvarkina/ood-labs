#ifndef FLYWITHWINGS_H
#define FLYWITHWINGS_H

#include "IFlyBehavior.h"
#include <iostream>
#include <memory>
#include <algorithm>
#include <functional>

FlyBehavior MakeFlyWithWings()
{
	//auto flightCounter = std::make_shared<int>(0);
	int flightCounter = 0;

	return 
	{
		[flightCounter]() mutable 
		{
			++(flightCounter);
			std::cout << "I'm flying with wings!!" << std::endl;
			std::cout << "The flight number is " << flightCounter << std::endl;
		},
		[flightCounter]() 
		{
			return flightCounter;
		},
		[]() 
		{ 
			return true; 
		}
	};
}

#endif