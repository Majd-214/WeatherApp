#include "Forecast.h"
#include <iostream>
using namespace std;

// Default constructor
Forecast::Forecast() : Weather() {
    for (int i = 0; i < 24; i++) {
        // Initialize each hour with default values
        hour[i].setHour(i);
        hour[i].setTemperature(0.0);
        hour[i].setCondition("Unknown");
    }
}

// temperature and condition
Forecast::Forecast(double defaultTemp, const string &defaultCond)
    : Weather(0, defaultTemp, defaultCond) // Calls base class
{
    for (int i = 0; i < 24; i++) {
        hour[i].setHour(i);
        hour[i].setTemperature(defaultTemp);
        hour[i].setCondition(defaultCond);
    }
}

void Forecast::setHourlyWeather(int h, double temp, const string &cond) {
    if (h >= 0 && h < 24) {
        hour[h].setHour(h);
        hour[h].setTemperature(temp);
        hour[h].setCondition(cond);
    }
}

Weather Forecast::getHourlyWeather(int h) const {
    if (h >= 0 && h < 24) {
        return hour[h];
    }
    // If out of range, return a default Weather object
    return Weather();
}

void Forecast::display() const {
    cout << "24-Hour Forecast:" << endl;
    for (int i = 0; i < 24; i++) {
        hour[i].display();
    }
}
