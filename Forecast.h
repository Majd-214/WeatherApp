// Forecast.h
#ifndef FORECAST_H
#define FORECAST_H

#include "Weather.h" // Dependency for weather data container
#include <vector>    // For storing lists of forecasts
#include <string>    // For date and time strings

// Represents weather conditions for a specific hour within a day.
class HourlyForecast {
private:
    Weather weather; // Weather data for this hour.
    std::string time; // Time identifier (e.g., "HH:MM").

public:
    // Constructor: Initializes with weather data and time string.
    // Takes Weather by const reference and copies it, or could be modified to move.
    HourlyForecast(Weather w, const std::string& t) : weather(std::move(w)), time(t) {} // Now moves Weather
    // Provides read-only access to the hourly weather data.
    const Weather& getWeather() const { return weather; }
    // Provides read-only access to the time string.
    std::string getTime() const { return time; }
};

// Represents the forecast for a single day, containing daily summary and hourly details.
class DailyForecast {
private:
    std::string date; // Date identifier (e.g., "YYYY-MM-DD").
    Weather dayWeather; // Summary weather data for the entire day.
    std::vector<HourlyForecast> hourlyForecasts; // List of hourly forecasts for this day.

public:
    // Constructor: Initializes with date and daily summary weather.
    // Takes Weather by const reference and copies it, or could be modified to move.
    DailyForecast(const std::string& d, Weather dw) : date(d), dayWeather(std::move(dw)) {} // Now moves Weather
    // Adds an hourly forecast entry to this day's list.
    // Takes HourlyForecast by const reference and copies it, or could be modified to move.
    void addHourlyForecast(HourlyForecast forecast) { // Now takes by value and moves
        hourlyForecasts.push_back(std::move(forecast));
    }
    // Provides read-only access to the vector of hourly forecasts.
    const std::vector<HourlyForecast>& getHourlyForecasts() const { return hourlyForecasts; }
    // Provides read-only access to the daily summary weather data.
    const Weather& getDayWeather() const { return dayWeather; }
    // Provides read-only access to the date string.
    std::string getDate() const { return date; }
};

// Top-level container for the entire forecast period, holding multiple daily forecasts.
class Forecast {
private:
    std::vector<DailyForecast> dailyForecasts; // List of daily forecasts.

public:
    // Adds a daily forecast entry to the main list.
    // Takes DailyForecast by const reference and copies it, or could be modified to move.
    void addDailyForecast(DailyForecast forecast) { // Now takes by value and moves
        dailyForecasts.push_back(std::move(forecast));
    }
    // Provides read-only access to the vector of daily forecasts.
    const std::vector<DailyForecast>& getDailyForecasts() const { return dailyForecasts; }

    // Note: Display methods previously here are now moved to ForecastReport.
};

#endif // FORECAST_H