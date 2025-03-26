#include "Forecast.h"
#include "Weather.h"
#include <iostream>
#include <string>
using namespace std;

Forecast::Forecast() : Weather() {
    // Initialize each hour with defaults
    for (int i = 0; i < 24; i++) {
        hourly[i].setHour(i);
        hourly[i].setTemperature(0.0);
        hourly[i].setCondition("Unknown");
    }
}

Forecast::Forecast(double defaultTemp, const string &defaultCond)
    : Weather(0, defaultTemp, defaultCond)
{
    // Initialize each hour to the given defaults
    for (int i = 0; i < 24; i++) {
        hourly[i].setHour(i);
        hourly[i].setTemperature(defaultTemp);
        hourly[i].setCondition(defaultCond);
    }
}

void Forecast::setHourlyWeather(int h, double temp, const string &cond) {
    if (h >= 0 && h < 24) {
        hourly[h].setHour(h);
        hourly[h].setTemperature(temp);
        hourly[h].setCondition(cond);
    }
}

Weather Forecast::getHourlyWeather(int h) const {
    if (h >= 0 && h < 24) {
        return hourly[h];
    }
    // Return a default Weather if hour is invalid
    return Weather();#include "Forecast.h"
#include "Weather.h"
#include <iostream>
#include <string>
using namespace std;

Forecast::Forecast() : Weather() {
    // Initialize each hour with defaults
    for (int i = 0; i < 24; i++) {
        hourly[i].setHour(i);
        hourly[i].setTemperature(0.0);
        hourly[i].setCondition("Unknown");
    }
}

Forecast::Forecast(double defaultTemp, const string &defaultCond)
    : Weather(0, defaultTemp, defaultCond)
{
    // Initialize each hour to the given defaults
    for (int i = 0; i < 24; i++) {
        hourly[i].setHour(i);
        hourly[i].setTemperature(defaultTemp);
        hourly[i].setCondition(defaultCond);
    }
}

void Forecast::setHourlyWeather(int h, double temp, const string &cond) {
    if (h >= 0 && h < 24) {
        hourly[h].setHour(h);
        hourly[h].setTemperature(temp);
        hourly[h].setCondition(cond);
    }
}

Weather Forecast::getHourlyWeather(int h) const {
    if (h >= 0 && h < 24) {
        return hourly[h];
    }
    // Return a default Weather if hour is invalid
    return Weather();
}

void Forecast::display() const {
    std::cout << "24-Hour Forecast:\n";
    for (int i = 0; i < 24; i++) {
        hourly[i].display();
    }
}

}

void Forecast::display() const {
    std::cout << "24-Hour Forecast:\n";
    for (int i = 0; i < 24; i++) {
        hourly[i].display();
    }
}
