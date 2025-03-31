// Forecast.h
#ifndef FORECAST_H
#define FORECAST_H
#include "Weather.h"

class ForecastDay {
    private:
    std::string date;
    Weather* hourlyForecasts;
    int numHourlyForecasts;

    public:
    ForecastDay(const std::string& d, int numHours);
    ~ForecastDay();
    void addHourlyForecast(int hour, const Weather& forecast);
    const Weather& getHourlyForecast(int hour) const;
    int getNumHourlyForecasts() const;
    std::string getDate() const;
};

class Forecast {
    private:
    ForecastDay** forecastDays;
    int numForecastDays;

    public:
    Forecast(int numDays);
    ~Forecast();
    void addForecastDay(int dayIndex, ForecastDay* day);
    const ForecastDay* getForecastDay(int dayIndex) const;
    int getNumForecastDays() const;
    void display() const;
};

#endif // FORECAST_H