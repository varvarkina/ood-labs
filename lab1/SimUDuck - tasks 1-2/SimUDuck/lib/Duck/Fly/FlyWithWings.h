#ifndef FLYWITHWINGS_H
#define FLYWITHWINGS_H

#include "IFlyBehavior.h"
#include <iostream>

class FlyWithWings : public IFlyBehavior
{
public:
	void Fly() override
	{
		m_flightCounter++;
		std::cout << "I'm flying with wings!!" << std::endl;
		std::cout << "The flight number is " << m_flightCounter << std::endl;
	}

	int GetFlightCounter() const override 
	{
		return m_flightCounter;
	}

	bool CanFly() const override
	{
		return true;
	}

private:
	int m_flightCounter = 0;
};

#endif