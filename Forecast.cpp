// Forecast.cpp
#include "Forecast.h"
#include <iostream>
#include <iomanip>

ForecastDay::ForecastDay(const std::string& d, int numHours) : date(d), numHourlyForecasts(numHours) {
    hourlyForecasts = new Weather[numHourlyForecasts];
}

ForecastDay::~ForecastDay() {
    delete[] hourlyForecasts;
}

void ForecastDay::addHourlyForecast(int hour, const Weather& forecast) {
    if (hour >= 0 && hour < numHourlyForecasts) {
        hourlyForecasts[hour] = forecast;
    }
}

const Weather& ForecastDay::getHourlyForecast(int hour) const {
    if (hour >= 0 && hour < numHourlyForecasts) {
        return hourlyForecasts[hour];
    }
    // Return a default Weather object if out of range
    return Weather();
}

int ForecastDay::getNumHourlyForecasts() const {
    return numHourlyForecasts;
}

std::string ForecastDay::getDate() const {
    return date;
}

Forecast::Forecast(int numDays) : numForecastDays(numDays) {
    forecastDays = new ForecastDay*[numForecastDays];
    for (int i = 0; i < numForecastDays; ++i) {
        forecastDays[i] = nullptr;
    }
}

Forecast::~Forecast() {
    for (int i = 0; i < numForecastDays; ++i) {
        delete forecastDays[i];
    }
    delete[] forecastDays;
}

void Forecast::addForecastDay(int dayIndex, ForecastDay* day) {
    if (dayIndex >= 0 && dayIndex < numForecastDays) {
        forecastDays[dayIndex] = day;
    }
}

const ForecastDay* Forecast::getForecastDay(int dayIndex) const {
    if (dayIndex >= 0 && dayIndex < numForecastDays) {
        return forecastDays[dayIndex];
    }
    return nullptr;
}

int Forecast::getNumForecastDays() const {
    return numForecastDays;
}

void Forecast::display() const {
    std::cout << "\nWeather Forecast:" << std::endl;
    for (int i = 0; i < numForecastDays; ++i) {
        const ForecastDay* day = forecastDays[i];
        if (day) {
            std::cout << "  Date: " << day->getDate() << std::endl;
            for (int j = 0; j < day->getNumHourlyForecasts(); ++j) {
                const Weather& hour = day->getHourlyForecast(j);
                std::cout << "    Time: " << hour.getProperty(LAST_UPDATED)->getValue() << " (Epoch)" << std::endl;
                for (int k = 0; k < NUM_PROPERTIES; ++k) {
                    Property* prop = hour.getProperty(static_cast<PropertyIndex>(k));
                    if (prop != nullptr) {
                        std::cout << "      " << std::left << std::setw(15) << prop->getName() << ": " << std::fixed << std::setprecision(2) << prop->getValue();
                        if (!prop->getUnit().empty()) {
                            std::cout << " " << prop->getUnit();
                        }
                        std::cout << std::endl;
                    }
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }
    }
}