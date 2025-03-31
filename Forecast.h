// Forecast.h
#ifndef FORECAST_H
#define FORECAST_H
#include "Weather.h"
#include <vector>

class HourlyForecast {
private:
    Weather weather;
    std::string time;

public:
    HourlyForecast(const Weather& w, const std::string& t) : weather(w), time(t) {}
    const Weather& getWeather() const { return weather; }
    std::string getTime() const { return time; }
};

class DailyForecast {
private:
    std::string date;
    Weather dayWeather;
    std::vector<HourlyForecast> hourlyForecasts;

public:
    DailyForecast(const std::string& d, const Weather& dw) : date(d), dayWeather(dw) {}
    void addHourlyForecast(const HourlyForecast& forecast) {
        hourlyForecasts.push_back(forecast);
    }
    const std::vector<HourlyForecast>& getHourlyForecasts() const { return hourlyForecasts; }
    const Weather& getDayWeather() const { return dayWeather; }
    std::string getDate() const { return date; }
};

class Forecast {
private:
    std::vector<DailyForecast> dailyForecasts;

public:
    void addDailyForecast(const DailyForecast& forecast) {
        dailyForecasts.push_back(forecast);
    }
    const std::vector<DailyForecast>& getDailyForecasts() const { return dailyForecasts; }
    void displayHourly() const;
    void displayDaily() const;
};

#endif // FORECAST_H