#define CATCH_CONFIG_MAIN
#include "../../../../catch2/catch.hpp"
#include "../../WeatherStation/WeatherStation/Observer.h"
#include "../../WeatherStation/WeatherStation/WeatherData.h"

class RemovingObserver : public IObserver<SWeatherInfo>
{
public:
    explicit RemovingObserver(IObservable<SWeatherInfo>& observable)
        : m_observable(observable)
    {
    }

    void Update(SWeatherInfo const& data) override
    {
        m_observable.RemoveObserver(*this);
    }

private:
    IObservable<SWeatherInfo>& m_observable;
};

TEST_CASE("Self-removing observer doesn't cause undefined behavior")
{
    CWeatherData wd;
    RemovingObserver selfRemovingObserver(wd);

    wd.RegisterObserver(selfRemovingObserver);

    REQUIRE_NOTHROW(wd.SetMeasurements(5, 0.6, 761));
    REQUIRE_NOTHROW(wd.SetMeasurements(10, 0.5, 762));
}