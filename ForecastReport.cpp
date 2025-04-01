// ForecastReport.cpp
#include "ForecastReport.h"
#include "Weather.h"    // For accessing Weather properties
#include "Property.h"   // For accessing Property details
#include <utility>      // For std::move
#include <ostream>      // For std::ostream
#include <iomanip>      // For std::setw, std::left, std::right, std::fixed, std::setprecision
#include <sstream>      // Potentially for formatting strings

// Constructor (remains the same)
ForecastReport::ForecastReport(Forecast data, DetailLevel level)
    : forecastData(std::move(data)), displayLevel(level) {}

// getReportType (remains the same)
std::string ForecastReport::getReportType() const {
    return (displayLevel == DetailLevel::HOURLY) ? "Hourly Forecast" : "Daily Forecast Summary";
}

// Main display function (remains the same)
void ForecastReport::display(std::ostream& os) const {
    os << "\n--- " << getReportType() << " ---" << std::endl;
    if (forecastData.getDailyForecasts().empty()) {
        os << "(No forecast data available)" << std::endl;
    } else {
        if (displayLevel == DetailLevel::HOURLY) {
            displayHourly(os);
        } else {
            displayDaily(os);
        }
    }
    os << "\n--- End of " << getReportType() << " ---" << std::endl;
}

// getForecast (remains the same)
const Forecast& ForecastReport::getForecast() const {
    return forecastData;
}

// --- Private Display Helpers ---

// displayDaily (remains the same - uses displayWeatherForForecast helper)
void displayWeatherForForecast(std::ostream& os, const Weather& weather); // Forward declare helper

void ForecastReport::displayDaily(std::ostream& os) const {
    const auto& dailyForecasts = forecastData.getDailyForecasts();
    // Guard clause already handled in main display method
    for (const auto& day : dailyForecasts) {
        os << "\n  Date: " << day.getDate() << std::endl;
        displayWeatherForForecast(os, day.getDayWeather());
    }
}

// **MODIFIED displayHourly for Tabular Format**
void ForecastReport::displayHourly(std::ostream& os) const {
    const auto& dailyForecasts = forecastData.getDailyForecasts();
    // Guard clause already handled in main display method

    // Define column widths
    const int timeW = 6;
    const int tempW = 7; // e.g., "-10.5C"
    const int feelW = 7;
    const int windW = 12; // e.g., "25.0km/h N"
    const int precW = 8; // e.g., "10.5mm"
    const int cldW = 7;  // e.g., "100%"

    for (const auto& day : dailyForecasts) {
        os << "\n  Date: " << day.getDate() << std::endl;
        const auto& hourlyForecasts = day.getHourlyForecasts();
         if (hourlyForecasts.empty()) {
             os << "    (No hourly data for this day)" << std::endl;
             continue;
         }

        // Print Table Header
        os << "  " << left
           << setw(timeW) << "Time" << "| "
           << setw(tempW) << "Temp" << "| "
           << setw(feelW) << "Feels" << "| "
           << setw(windW) << "Wind" << "| "
           << setw(precW) << "Precip" << "| "
           << setw(cldW) << "Cloud" // << "| Condition" // Add if condition available
           << endl;
        os << "  " << string(timeW, '-') << "+"
           << string(tempW+1, '-') << "+" // +1 for space
           << string(feelW+1, '-') << "+"
           << string(windW+1, '-') << "+"
           << string(precW+1, '-') << "+"
           << string(cldW+1, '-') //<< "+----------"
           << endl;

        // Print Table Rows
        for (const auto& hour : hourlyForecasts) {
            const Weather& hw = hour.getWeather(); // Get weather for the hour

            // Extract specific properties (use default values if null)
            Property* tempProp = hw.getProperty(TEMPERATURE);
            Property* feelProp = hw.getProperty(FEELS_LIKE);
            Property* windSpdProp = hw.getProperty(WIND_SPEED);
            Property* windDirProp = hw.getProperty(WIND_DIRECTION); // Get direction too
            Property* precProp = hw.getProperty(PRECIPITATION);
            Property* cldProp = hw.getProperty(CLOUD);
            // Property* condProp = hw.getProperty(CONDITION); // If you add condition

            // Format values safely
            stringstream ssTemp, ssFeel, ssWind, ssPrec, ssCld; // , ssCond;

            if(tempProp) ssTemp << fixed << setprecision(1) << tempProp->getValue() << tempProp->getUnit(); else ssTemp << "N/A";
            if(feelProp) ssFeel << fixed << setprecision(1) << feelProp->getValue() << feelProp->getUnit(); else ssFeel << "N/A";
            if(precProp) ssPrec << fixed << setprecision(1) << precProp->getValue() << precProp->getUnit(); else ssPrec << "N/A";
            if(cldProp) ssCld << fixed << setprecision(0) << cldProp->getValue() << cldProp->getUnit(); else ssCld << "N/A";
            // Combine wind speed and direction (simple cardinal for now)
            if(windSpdProp) {
                ssWind << fixed << setprecision(1) << windSpdProp->getValue() << windSpdProp->getUnit();
                if(windDirProp) {
                    double deg = windDirProp->getValue();
                    string dir = "N"; // Basic direction
                    if(deg > 22.5 && deg <= 67.5) dir = "NE";
                    else if(deg > 67.5 && deg <= 112.5) dir = "E";
                    else if(deg > 112.5 && deg <= 157.5) dir = "SE";
                    else if(deg > 157.5 && deg <= 202.5) dir = "S";
                    else if(deg > 202.5 && deg <= 247.5) dir = "SW";
                    else if(deg > 247.5 && deg <= 292.5) dir = "W";
                    else if(deg > 292.5 && deg <= 337.5) dir = "NW";
                    ssWind << " " << dir;
                }
            } else { ssWind << "N/A"; }
            // if(condProp) ssCond << condProp->getName(); else ssCond << "N/A"; // Assuming condition stored as name/string

            // Print formatted row
            os << "  " << left
               << setw(timeW) << hour.getTime() << "| " << right // Align numbers right
               << setw(tempW) << ssTemp.str() << "| "
               << setw(feelW) << ssFeel.str() << "| " << left // Align wind left maybe
               << setw(windW) << ssWind.str() << "| " << right
               << setw(precW) << ssPrec.str() << "| "
               << setw(cldW) << ssCld.str() // << "| " << left << ssCond.str()
               << endl;
        }
    }
}


// Definition for the helper used by displayDaily (if needed outside this file, declare in header)
// Otherwise, keep its definition static or in anonymous namespace if only used here.
void displayWeatherForForecast(std::ostream& os, const Weather& weather) {
     bool dataDisplayed = false;
     for (int k = 0; k < NUM_PROPERTIES; ++k) {
        const Property* prop = weather.getProperty(static_cast<PropertyIndex>(k));
        if (prop != nullptr && static_cast<PropertyIndex>(k) != LAST_UPDATED) {
             if (prop->getValue() != 0 || static_cast<PropertyIndex>(k) == TEMPERATURE || static_cast<PropertyIndex>(k) == FEELS_LIKE) {
                 os << "      " << std::left << std::setw(15) << prop->getName() << ": "
                    << std::fixed << std::setprecision(1)
                    << prop->getValue();
                if (!prop->getUnit().empty()) {
                     os << " " << prop->getUnit();
                }
                 os << std::endl;
                 dataDisplayed = true;
             }
        }
    }
     if (!dataDisplayed) {
         os << "      (No specific forecast details available)" << std::endl;
     }
}