//
// Created by Majd Aburas on 18-Mar-2025.
//

#ifndef WEATHERAPP_WEATHER_H_
#define WEATHERAPP_WEATHER_H_

#include <iostream>
#include <string>

#include "Property.h"

using namespace std;

class Weather {
private:
    string test;
    Property temperature;
    Property humidity;
    Property windSpeed;
    Property windDirection;

public:
    Weather();

    Weather(string test);
};

#endif //WEATHERAPP_WEATHER_H_
