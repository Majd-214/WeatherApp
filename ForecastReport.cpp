// ForecastReport.cpp
#include "ForecastReport.h"
#include "Weather.h"    // Needed for accessing Weather data within Forecast
#include "Property.h"   // Needed for accessing Property details within Weather
#include <utility>      // For std::move
#include <ostream>      // For std::ostream
#include <iomanip>      // For stream manipulators (formatting output)
#include <sstream>      // For formatting values into strings
#include <string>       // Included via headers, but good practice

// --- Helper Function Declaration ---

// Helper to display weather data in a consistent format for forecasts.
// Declared static as it's only used within this file.
static void displayWeatherForForecast(std::ostream& os, const Weather& weather);

// --- Constructor ---

// Initializes the report by moving the forecast data and setting the detail level.
ForecastReport::ForecastReport(Forecast data, DetailLevel level)
    : forecastData(std::move(data)), displayLevel(level) {}

// --- Overridden Methods ---

// Returns the report type string based on the selected display level.
std::string ForecastReport::getReportType() const {
    return (displayLevel == DetailLevel::HOURLY) ? "Hourly Forecast" : "Daily Forecast Summary";
}

// Main display entry point. Prints header/footer and calls the appropriate helper.
void ForecastReport::display(std::ostream& os) const {
    os << "\n--- " << getReportType() << " ---" << std::endl;
    if (forecastData.getDailyForecasts().empty()) {
        os << "(No forecast data available)" << std::endl;
    } else {
        // Delegate to the specific display function based on level.
        if (displayLevel == DetailLevel::HOURLY) {
            displayHourly(os);
        } else {
            displayDaily(os);
        }
    }
    os << "--- End of " << getReportType() << " ---" << std::endl;
}

// --- Specific Getter ---

// Provides const access to the contained Forecast data object.
const Forecast& ForecastReport::getForecast() const {
    return forecastData;
}

// --- Private Display Helpers ---

// Displays daily summary forecast information.
void ForecastReport::displayDaily(std::ostream& os) const {
    const auto& dailyForecasts = forecastData.getDailyForecasts();
    // Iterates through each day and displays its summary using the helper.
    for (const auto& day : dailyForecasts) {
        os << "\n  Date: " << day.getDate() << std::endl;
        displayWeatherForForecast(os, day.getDayWeather()); // Use helper
    }
}

// Displays hourly forecast information in a formatted table.
void ForecastReport::displayHourly(std::ostream& os) const {
    const auto& dailyForecasts = forecastData.getDailyForecasts();

    // Define fixed column widths for alignment.
    const int timeW = 6;    // "HH:MM"
    const int tempW = 7;    // "XXX.XC"
    const int feelW = 7;    // "XXX.XC"
    const int windW = 12;   // "XX.Xkm/h NW"
    const int precW = 8;    // "XX.Xmm"
    const int cldW  = 7;    // "100%"

    // Iterate through each day in the forecast.
    for (const auto& day : dailyForecasts) {
        os << "\n  Date: " << day.getDate() << std::endl;
        const auto& hourlyForecasts = day.getHourlyForecasts();
         if (hourlyForecasts.empty()) {
             os << "    (No hourly data for this day)" << std::endl;
             continue; // Skip to the next day
         }

        // Print the table header row.
        os << "  " << left
           << setw(timeW) << "Time" << "| "
           << setw(tempW) << "Temp" << "| "
           << setw(feelW) << "Feels" << "| "
           << setw(windW) << "Wind" << "| "
           << setw(precW) << "Precip" << "| "
           << setw(cldW) << "Cloud"
           << endl;
        // Print the separator line.
        os << "  " << string(timeW, '-') << "+"
           << string(tempW+1, '-') << "+" // +1 for the space
           << string(feelW+1, '-') << "+"
           << string(windW+1, '-') << "+"
           << string(precW+1, '-') << "+"
           << string(cldW+1, '-')
           << endl;

        // Print each hour's data as a table row.
        for (const auto& hour : hourlyForecasts) {
            const Weather& hw = hour.getWeather(); // Get Weather object for the hour.

            // Safely get pointers to required properties (might be null).
            Property* tempProp = hw.getProperty(TEMPERATURE);
            Property* feelProp = hw.getProperty(FEELS_LIKE);
            Property* windSpdProp = hw.getProperty(WIND_SPEED);
            Property* windDirProp = hw.getProperty(WIND_DIRECTION);
            Property* precProp = hw.getProperty(PRECIPITATION);
            Property* cldProp = hw.getProperty(CLOUD);

            // Use stringstreams to format each column's value safely.
            stringstream ssTemp, ssFeel, ssWind, ssPrec, ssCld;

            if(tempProp) ssTemp << fixed << setprecision(1) << tempProp->getValue() << tempProp->getUnit(); else ssTemp << "N/A";
            if(feelProp) ssFeel << fixed << setprecision(1) << feelProp->getValue() << feelProp->getUnit(); else ssFeel << "N/A";
            if(precProp) ssPrec << fixed << setprecision(1) << precProp->getValue() << precProp->getUnit(); else ssPrec << "N/A";
            if(cldProp) ssCld << fixed << setprecision(0) << cldProp->getValue() << cldProp->getUnit(); else ssCld << "N/A";

            // Format wind: combine speed and cardinal direction.
            if(windSpdProp) {
                ssWind << fixed << setprecision(1) << windSpdProp->getValue() << windSpdProp->getUnit();
                if(windDirProp) {
                    // Convert wind degrees to a cardinal direction string.
                    double deg = windDirProp->getValue();
                    string dir = "N"; // Default North
                    if(deg > 22.5 && deg <= 67.5) dir = "NE";
                    else if(deg > 67.5 && deg <= 112.5) dir = "E";
                    else if(deg > 112.5 && deg <= 157.5) dir = "SE";
                    else if(deg > 157.5 && deg <= 202.5) dir = "S";
                    else if(deg > 202.5 && deg <= 247.5) dir = "SW";
                    else if(deg > 247.5 && deg <= 292.5) dir = "W";
                    else if(deg > 292.5 && deg <= 337.5) dir = "NW";
                    // else stays "N" for 337.5 to 22.5
                    ssWind << " " << dir;
                }
            } else { ssWind << "N/A"; }

            // Print the formatted row using stream manipulators for alignment.
            os << "  " << left
               << setw(timeW) << hour.getTime() << "| " << right // Align numerical data right
               << setw(tempW) << ssTemp.str() << "| "
               << setw(feelW) << ssFeel.str() << "| " << left // Keep wind left-aligned
               << setw(windW) << ssWind.str() << "| " << right
               << setw(precW) << ssPrec.str() << "| "
               << setw(cldW) << ssCld.str()
               << endl;
        }
    }
}

// --- Static Helper Function Definition ---

// Helper function to display relevant weather properties for forecast summaries.
// Omits properties like LAST_UPDATED.
static void displayWeatherForForecast(std::ostream& os, const Weather& weather) {
     bool dataDisplayed = false;
     // Iterate through all possible property indices.
     for (int k = 0; k < NUM_PROPERTIES; ++k) {
        PropertyIndex currentIndex = static_cast<PropertyIndex>(k);
        const Property* prop = weather.getProperty(currentIndex);

        // Check if the property exists and is not LAST_UPDATED.
        if (prop != nullptr && currentIndex != LAST_UPDATED) {
             // Display temperature/feels like even if zero, otherwise only non-zero values.
             // (This logic could be refined based on desired output).
             if (prop->getValue() != 0.0 || currentIndex == TEMPERATURE || currentIndex == FEELS_LIKE) {
                 os << "      " // Indentation for forecast details
                    << std::left << std::setw(15) << prop->getName() << ": " // Property name
                    << std::fixed << std::setprecision(1)
                    << prop->getValue(); // Property value
                if (!prop->getUnit().empty()) { // Add unit if available
                     os << " " << prop->getUnit();
                }
                 os << std::endl;
                 dataDisplayed = true; // Mark that some data was shown
             }
        }
    }
     // If no relevant properties were displayed, print a placeholder.
     if (!dataDisplayed) {
         os << "      (No specific forecast details available)" << std::endl;
     }
}