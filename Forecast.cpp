// Forecast.cpp
#include "Forecast.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <ctime>

// Helper function to display properties of a Weather object (using std::cout)
void displayWeatherPropertiesForForecast(const Weather& weather) {
     bool dataDisplayed = false;
     for (int k = 0; k < NUM_PROPERTIES; ++k) {
        Property* prop = weather.getProperty(static_cast<PropertyIndex>(k));
        // Only display if property exists AND it's not the 'Last Updated' epoch time (less relevant for forecast)
        if (prop != nullptr && static_cast<PropertyIndex>(k) != LAST_UPDATED) {
            // Simple check to maybe hide zero values unless it's temperature
             if (prop->getValue() != 0 || static_cast<PropertyIndex>(k) == TEMPERATURE || static_cast<PropertyIndex>(k) == FEELS_LIKE)
             {
                 std::cout << "      " << std::left << std::setw(15) << prop->getName() << ": "
                           << std::fixed << std::setprecision(1) // Consistent precision
                           << prop->getValue();
                // Append unit only if it's not empty
                if (!prop->getUnit().empty()) {
                     std::cout << " " << prop->getUnit();
                }
                 std::cout << std::endl;
                 dataDisplayed = true;
             }
        }
    }
     if (!dataDisplayed) {
         std::cout << "      (No specific forecast details available)" << std::endl;
     }
}


void Forecast::displayHourly() const {
    std::cout << "\n--- Hourly Forecast ---" << std::endl;
    if (dailyForecasts.empty()) {
        std::cout << "No hourly forecast data available." << std::endl;
        return;
    }
    for (const auto& day : dailyForecasts) {
        std::cout << "\n  Date: " << day.getDate() << std::endl;
         if (day.getHourlyForecasts().empty()) {
             std::cout << "    No hourly data for this day." << std::endl;
             continue;
         }
        for (const auto& hour : day.getHourlyForecasts()) {
            std::cout << "    Time: " << hour.getTime() << std::endl;
            displayWeatherPropertiesForForecast(hour.getWeather()); // Use helper
        }
    }
     std::cout << "\n--- End of Hourly Forecast ---" << std::endl;
}

void Forecast::displayDaily() const {
    std::cout << "\n--- Daily Forecast Summary ---" << std::endl;
     if (dailyForecasts.empty()) {
        std::cout << "No daily forecast data available." << std::endl;
        return;
    }
    for (const auto& day : dailyForecasts) {
        std::cout << "\n  Date: " << day.getDate() << std::endl;
        displayWeatherPropertiesForForecast(day.getDayWeather()); // Use helper
    }
     std::cout << "\n--- End of Daily Forecast ---" << std::endl;
}