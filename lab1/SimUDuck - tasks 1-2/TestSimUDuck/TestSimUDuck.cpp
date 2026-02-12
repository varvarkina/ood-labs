#define CATCH_CONFIG_MAIN
#include "../../../catch2/catch.hpp"
#include "../SimUDuck/lib/Duck/Duck.h"
#include "../SimUDuck/lib/Duck/Fly/IFlyBehavior.h"
#include "../SimUDuck/lib/Duck/Quack/IQuakBehavior.h"
#include "../SimUDuck/lib/Duck/Dance/IDanceBehavior.h"
#include "../SimUDuck/lib/Duck/Fly/FlyWithWings.h"
#include "../SimUDuck/lib/Duck/Fly/FlyNoWay.h"

#include <memory>

class MockFlyBehavior : public IFlyBehavior
{
public:
	MockFlyBehavior(bool canFly)
		: m_canFly(canFly) {}

	void Fly() override
	{
		if (m_canFly) m_flightCounter++;
	}

	int GetFlightCounter() const override
	{
		return m_flightCounter;
	}

	bool CanFly() const override
	{
		return m_canFly;
	}

private:
	bool m_canFly;
	int m_flightCounter = 0;
};

class MockQuackBehavior : public IQuackBehavior 
{
public:
	MockQuackBehavior(bool canQuack) : m_canQuack(canQuack) {}
	
	void Quack() override 
	{ 
		if (m_canQuack)
		{
			m_quackCalled = true;
		}
	}

	bool WasQuackCalled() const 
	{ 
		return m_quackCalled; 
	}

	bool CanQuack() const 
	{ 
		return m_canQuack; 
	}

	void Reset() 
	{ 
		m_quackCalled = false; 
	}

private:
	bool m_canQuack;
	bool m_quackCalled = false;
};

class MockDanceBehavior : public IDanceBehavior
{
public:
    void Dance() const override 
	{ 
		m_danceCalled = true; 
	}

	bool WasDanceCalled() const
	{
		return m_danceCalled;
	}

private:
    mutable bool m_danceCalled = false;
};

class TestDuck : public Duck
{
public:
	TestDuck(std::unique_ptr<IFlyBehavior>&& flyBehavior,
		std::unique_ptr<IQuackBehavior>&& quackBehavior,
		std::unique_ptr<IDanceBehavior>&& danceBehavior)
		: Duck(std::move(flyBehavior), std::move(quackBehavior), std::move(danceBehavior))
	{
	}

	void Display() const override {}
};

TEST_CASE("Dance behavior test")
{
	auto flyBehavior = std::make_unique<MockFlyBehavior>(true);
	auto quackBehavior = std::make_unique<MockQuackBehavior>(true);
	auto danceBehavior = std::make_unique<MockDanceBehavior>();

	auto* danceMock = danceBehavior.get();

	TestDuck duck(std::move(flyBehavior), std::move(quackBehavior), std::move(danceBehavior));

	REQUIRE_FALSE(danceMock->WasDanceCalled());

	duck.Dance();
	REQUIRE(danceMock->WasDanceCalled());
}

TEST_CASE("Flying ducks quack after every second flight")
{
	SECTION("Duck can fly and quack behavior")
	{
		auto flyBehavior = std::make_unique<FlyWithWings>();
		auto quackBehavior = std::make_unique<MockQuackBehavior>(true);
		auto danceBehavior = std::make_unique<MockDanceBehavior>();

		auto* flyMock = flyBehavior.get();
		auto* quackMock = quackBehavior.get();

		TestDuck duck(std::move(flyBehavior), std::move(quackBehavior), std::move(danceBehavior));

		duck.Fly();
		REQUIRE(flyMock->GetFlightCounter() == 1);
		REQUIRE_FALSE(quackMock->WasQuackCalled());

		duck.Fly();
		REQUIRE(flyMock->GetFlightCounter() == 2);
		REQUIRE(quackMock->WasQuackCalled());

		quackMock->Reset();

		duck.Fly();
		REQUIRE(flyMock->GetFlightCounter() == 3);
		REQUIRE_FALSE(quackMock->WasQuackCalled());

		duck.Fly();
		REQUIRE(flyMock->GetFlightCounter() == 4);
		REQUIRE(quackMock->WasQuackCalled());
	}

	SECTION("Duck can fly and can't quack behavior")
	{
		auto flyBehavior = std::make_unique<FlyWithWings>();
		auto quackBehavior = std::make_unique<MockQuackBehavior>(false); 
		auto danceBehavior = std::make_unique<MockDanceBehavior>();

		auto* flyMock = flyBehavior.get();
		auto* quackMock = quackBehavior.get();

		TestDuck duck(std::move(flyBehavior), std::move(quackBehavior), std::move(danceBehavior));

		for (int i = 0; i < 5; ++i)
		{
			duck.Fly();
			REQUIRE_FALSE(quackMock->WasQuackCalled());
		}

		REQUIRE(flyMock->GetFlightCounter() == 5);
	}
}

TEST_CASE("Non-flying ducks never quack during flight")
{
	SECTION("Duck can't fly and can quack behavior")
	{
		auto flyBehavior = std::make_unique<FlyNoWay>();
		auto quackBehavior = std::make_unique<MockQuackBehavior>(true);
		auto danceBehavior = std::make_unique<MockDanceBehavior>();

		auto* flyMock = flyBehavior.get();
		auto* quackMock = quackBehavior.get();

		TestDuck duck(std::move(flyBehavior), std::move(quackBehavior), std::move(danceBehavior));

		for (int i = 0; i < 5; ++i)
		{
			duck.Fly();
			REQUIRE(flyMock->GetFlightCounter() == 0);
			REQUIRE_FALSE(quackMock->WasQuackCalled());
		}
	}

	SECTION("Duck can't fly or quack behavior")
	{
		auto flyBehavior = std::make_unique<FlyNoWay>();
		auto quackBehavior = std::make_unique<MockQuackBehavior>(false);
		auto danceBehavior = std::make_unique<MockDanceBehavior>();

		auto* flyMock = flyBehavior.get();
		auto* quackMock = quackBehavior.get();

		TestDuck duck(std::move(flyBehavior), std::move(quackBehavior), std::move(danceBehavior));

		for (int i = 0; i < 3; ++i)
		{
			duck.Fly();
			REQUIRE(flyMock->GetFlightCounter() == 0);
			REQUIRE_FALSE(quackMock->WasQuackCalled());
		}
	}
}