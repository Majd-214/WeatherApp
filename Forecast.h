//
// Created by Joshua Kam on 2025-03-26.
//

#ifndef FORECAST_H
#define FORECAST_H
#include "Weather.h"

using namespace std;


class Forecast: public Weather {
private:
    //array for 24 hour forcast
    Weather hour[24];

public:
    Forecast();
    Forecast(double h, double temp, const string &cond);

    void setHourlyWeather(int hour, double temp, const std::string &cond);

    // Get the Weather object for a specific hour
    Weather getHourlyWeather(int hour) const;

    // Override display to show the entire 24-hour forecast
    void display() const override;
};




#endif //FORECAST_H
