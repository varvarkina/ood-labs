#ifndef DUCK_H
#define DUCK_H

#include "Fly/IFlyBehavior.h"
#include "Quack/IQuackBehavior.h"
#include "Dance/IDanceBehavior.h"

#include <cassert>
#include <iostream>
#include <memory>
#include <vector>

class Duck
{
public:
	Duck(FlyBehavior flyBehavior,
		QuackBehaviorStrategy quackBehavior,
		DanceBehavior danceBehavior)
		: m_quackBehavior(std::move(quackBehavior))
	{
		SetFlyBehavior(std::move(flyBehavior));
		SetDanceBehavior(std::move(danceBehavior));
	}

	void Quack() const
	{
		m_quackBehavior.Quack();
	}

	void Swim() const
	{
		std::cout << "I'm swimming" << std::endl;
	}

	void Fly()
	{
		if (!m_flyBehavior.CanFly())
		{
			return;
		}
		m_flyBehavior.Fly();

		int flightCounter = m_flyBehavior.GetFlightCounter();

		if (flightCounter % 2 == 0)
		{
			Quack();
		}
	}

	void Dance()
	{
		m_danceBehavior.Dance();
	}

	void SetFlyBehavior(FlyBehavior flyBehavior)
	{
		m_flyBehavior = std::move(flyBehavior);
	}

	void SetDanceBehavior(DanceBehavior danceBehavior)
	{
		m_danceBehavior = std::move(danceBehavior);
	}

	virtual void Display() const = 0;
	virtual ~Duck() = default;

private:
	FlyBehavior m_flyBehavior;
	QuackBehaviorStrategy m_quackBehavior;
	DanceBehavior m_danceBehavior;
};

#endif