#define CATCH_CONFIG_MAIN
#include "../../../../catch2/catch.hpp"
#include "../../3/Observer.h"
#include "../../3/WeatherData.h"
#include <vector>
#include <stdexcept>

struct MockObserver : IObserver<SWeatherInfo>
{
	int id;
	int updateCount = 0;
	std::vector<int>* callOrder = nullptr;
	bool throwOnUpdate = false;

	MockObserver(int id_, std::vector<int>* order = nullptr, bool shouldThrow = false)
		: id(id_), callOrder(order), throwOnUpdate(shouldThrow)
	{}

	void Update(SWeatherInfo const&) override
	{
		if (throwOnUpdate)
			throw std::runtime_error("update failed");

		++updateCount;
		if (callOrder)
			callOrder->push_back(id);
	}
};

TEST_CASE("Observers are notified in priority order", "[priority]")
{
	CWeatherData wd;
	std::vector<int> callOrder;

	MockObserver low(1, &callOrder);
	MockObserver mid(2, &callOrder);
	MockObserver high(3, &callOrder);

	wd.RegisterObserver(low, 1);   
	wd.RegisterObserver(mid, 5);   
	wd.RegisterObserver(high, 10); 

	wd.SetMeasurements(25, 0.5, 760);

	REQUIRE(callOrder == std::vector<int>{3, 2, 1});
}

TEST_CASE("Duplicate registration does not add the same observer twice", "[register]")
{
	CWeatherData wd;
	MockObserver obs(1);

	wd.RegisterObserver(obs, 5);
	wd.RegisterObserver(obs, 10); 

	wd.SetMeasurements(10, 0.5, 760);

	REQUIRE(obs.updateCount == 1); 
}

TEST_CASE("Observer is not notified after removal", "[remove]")
{
	CWeatherData wd;
	MockObserver obs(1);

	wd.RegisterObserver(obs, 2);
	wd.SetMeasurements(20, 0.7, 765);
	REQUIRE(obs.updateCount == 1);

	wd.RemoveObserver(obs);
	wd.SetMeasurements(25, 0.8, 770);

	REQUIRE(obs.updateCount == 1); 
}

TEST_CASE("Register and Remove are exception-safe", "[exceptions]")
{
	CWeatherData wd;
	std::vector<int> order;
	MockObserver obs1(1, &order);
	MockObserver obs2(2, &order);
	MockObserver badObs(99, &order, true);

	wd.RegisterObserver(obs1, 5);
	wd.RegisterObserver(obs2, 3);
	wd.RegisterObserver(badObs, 10);

	REQUIRE_THROWS(wd.SetMeasurements(10, 0.5, 760));

	wd.RemoveObserver(badObs);

	REQUIRE_NOTHROW(wd.SetMeasurements(15, 0.6, 761));

	REQUIRE(obs1.updateCount > 0);
	REQUIRE(obs2.updateCount > 0);
}

