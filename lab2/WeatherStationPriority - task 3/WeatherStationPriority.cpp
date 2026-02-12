#include "WeatherData.h"

int main()
{
	CWeatherData wd;

	CDisplay display;
	CStatsDisplay stats;

	wd.RegisterObserver(display, 1);
	wd.RegisterObserver(stats, 10);

	wd.SetMeasurements(3, 0.7, 760);
	wd.SetMeasurements(4, 0.8, 761);

	return 0;
}