//
// Created by Majd Aburas on 18-Mar-2025.
//

#ifndef WEATHERAPP_WEATHER_H_
#define WEATHERAPP_WEATHER_H_

#include <iostream>
#include <string>
#include <Property.h>

using namespace std;

class Weather
{
private:
    string test;

    Property UVIndex;
    Property Humidity;
    Property Temperature;

public:

    void setUVIndex();
    void setHumidity();
    void setTemperature();

    double getUVIndex()const;
    double getHumidity() const ;
    double getTemperature() const ;


};

#endif //WEATHERAPP_WEATHER_H_
// int index (1-10)
//string level (low moderate high)