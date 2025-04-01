// ForecastReport.cpp
#include "ForecastReport.h"
#include "Weather.h"    // Needed for accessing Weather data within Forecast
#include "Property.h"   // Needed for accessing Property details within Weather
#include <utility>      // For std::move
#include <ostream>      // For std::ostream
#include <iomanip>      // For stream manipulators (formatting output)
#include <sstream>      // For formatting values into strings
#include <string>       // For std::string
#include <vector>       // Included via Weather.h, good practice
#include <ctime>        // For date/time parsing and formatting (%A)
#include <cmath>        // For std::fmod in degreesToCardinal

// --- Helper Functions (Anonymous Namespace) ---
namespace {
    // Converts wind direction degrees to a cardinal direction string (e.g., N, NE, E).
    std::string degreesToCardinal(double degrees) {
        degrees = std::fmod(degrees, 360.0);
        if (degrees < 0) degrees += 360.0;
        const std::string directions[] = {"N", "NE", "E", "SE", "S", "SW", "W", "NW", "N"};
        int index = static_cast<int>((degrees + 22.5) / 45.0);
        return directions[index % 8];
    }

    // Parses a "YYYY-MM-DD" string and returns the full day name (e.g., "Tuesday").
    std::string getDayOfWeek(const std::string& dateStr) {
        std::tm timeinfo = {};
        std::istringstream ss(dateStr);
        ss >> std::get_time(&timeinfo, "%Y-%m-%d");
        if (ss.fail()) { return "Unknown Day"; }
        // mktime normalizes tm struct and sets tm_wday
        // Check for -1 which indicates an error during normalization
        if (std::mktime(&timeinfo) == -1) { return "Unknown Day"; }

        char buffer[20];
        if (std::strftime(buffer, sizeof(buffer), "%A", &timeinfo)) {
            return buffer;
        } else {
            return "Unknown Day";
        }
    }

} // end anonymous namespace

// --- Static Helper Function Declaration ---

// Helper to display weather data in a consistent format for daily forecasts.
static void displayWeatherForForecast(std::ostream& os, const Weather& weather);

// --- Constructor ---

ForecastReport::ForecastReport(Forecast data, DetailLevel level)
    : forecastData(std::move(data)), displayLevel(level) {}

// --- Overridden Methods ---

std::string ForecastReport::getReportType() const {
    return (displayLevel == DetailLevel::HOURLY) ? "Hourly Forecast" : "Daily Forecast Summary";
}

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
    os << "--- End of " << getReportType() << " ---" << std::endl;
}

// --- Specific Getter ---

const Forecast& ForecastReport::getForecast() const {
    return forecastData;
}

// --- Private Display Helpers ---

// Displays daily summary forecast information with improved formatting and day separation.
void ForecastReport::displayDaily(std::ostream& os) const {
    const auto& dailyForecasts = forecastData.getDailyForecasts();
    bool firstDay = true; // Flag to avoid separator before the first day
    for (const auto& day : dailyForecasts) {
        // Add a distinct separator line before each day (except the first).
        if (!firstDay) {
            os << "----------------------------------------" << std::endl; // Separator line
        }
        firstDay = false;

        // Get the date string and corresponding day name.
        std::string dateStr = day.getDate();
        std::string dayName = getDayOfWeek(dateStr); // Use helper
        // Print header including day name and date.
        os << dayName << " (" << dateStr << ")" << std::endl; // Removed the extra "---"
        // Delegate detailed display to the helper function.
        displayWeatherForForecast(os, day.getDayWeather());
    }
}

// Displays hourly forecast information in a formatted table.
// Wind direction in hourly remains cardinal only for table brevity.
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
        // Get date and day name for the header.
        std::string dateStr = day.getDate();
        std::string dayName = getDayOfWeek(dateStr);
        os << "\n--- Hourly for " << dayName << " (" << dateStr << ") ---" << std::endl;

        const auto& hourlyForecasts = day.getHourlyForecasts();
         if (hourlyForecasts.empty()) {
             os << "    (No hourly data for this day)" << std::endl;
             continue; // Skip to the next day
         }

        // Print the table header row. Use std:: qualifier.
        os << "  " << std::left
           << std::setw(timeW) << "Time" << "| "
           << std::setw(tempW) << "Temp" << "| "
           << std::setw(feelW) << "Feels" << "| "
           << std::setw(windW) << "Wind" << "| "
           << std::setw(precW) << "Precip" << "| "
           << std::setw(cldW) << "Cloud"
           << std::endl;
        // Print the separator line. Use std:: qualifier.
        os << "  " << std::string(timeW, '-') << "+"
           << std::string(tempW+1, '-') << "+" // +1 for the space
           << std::string(feelW+1, '-') << "+"
           << std::string(windW+1, '-') << "+"
           << std::string(precW+1, '-') << "+"
           << std::string(cldW+1, '-')
           << std::endl;

        // Print each hour's data as a table row.
        for (const auto& hour : hourlyForecasts) {
            const Weather& hw = hour.getWeather();

            Property* tempProp = hw.getProperty(TEMPERATURE);
            Property* feelProp = hw.getProperty(FEELS_LIKE);
            Property* windSpdProp = hw.getProperty(WIND_SPEED);
            Property* windDirProp = hw.getProperty(WIND_DIRECTION);
            Property* precProp = hw.getProperty(PRECIPITATION);
            Property* cldProp = hw.getProperty(CLOUD);

            std::stringstream ssTemp, ssFeel, ssWind, ssPrec, ssCld;

            if(tempProp) ssTemp << std::fixed << std::setprecision(1) << tempProp->getValue() << tempProp->getUnit(); else ssTemp << "N/A";
            if(feelProp) ssFeel << std::fixed << std::setprecision(1) << feelProp->getValue() << feelProp->getUnit(); else ssFeel << "N/A";
            if(precProp) ssPrec << std::fixed << std::setprecision(1) << precProp->getValue() << precProp->getUnit(); else ssPrec << "N/A";
            if(cldProp) ssCld << std::fixed << std::setprecision(0) << cldProp->getValue() << cldProp->getUnit(); else ssCld << "N/A";

            // Format wind for hourly table (still cardinal only)
            if(windSpdProp) {
                ssWind << std::fixed << std::setprecision(1) << windSpdProp->getValue() << windSpdProp->getUnit();
                if(windDirProp) {
                    ssWind << " " << degreesToCardinal(windDirProp->getValue());
                }
            } else { ssWind << "N/A"; }

            // Print the formatted row
            os << "  " << std::left
               << std::setw(timeW) << hour.getTime() << "| " << std::right
               << std::setw(tempW) << ssTemp.str() << "| "
               << std::setw(feelW) << ssFeel.str() << "| " << std::left
               << std::setw(windW) << ssWind.str() << "| " << std::right
               << std::setw(precW) << ssPrec.str() << "| "
               << std::setw(cldW) << ssCld.str()
               << std::endl;
        }
    }
}


// --- Static Helper Function Definition ---

// Helper function to display relevant weather properties for daily forecast summaries.
// Displays wind direction with both cardinal and degrees. No internal subtitles.
static void displayWeatherForForecast(std::ostream& os, const Weather& weather) {
    bool dataDisplayed = false; // Overall flag

    // Define the order properties should ideally be displayed in for the daily summary
    const std::vector<PropertyIndex> dailyDisplayOrder = {
        TEMPERATURE, // Typically Avg Temp for daily
        WIND_SPEED,  // Typically Max Wind for daily
        WIND_DIRECTION, // Daily direction if available (using helper below assumes it exists)
        PRECIPITATION, // Total Precip
        HUMIDITY,     // Avg Humidity
        VISIBILITY,   // Avg Visibility
        UV            // Max UV
        // Add Feels Like, Gust, Cloud, Pressure here if the API provides *meaningful* daily summaries for them
    };

    // Lambda to print a specific property if it exists, used for brevity
    auto printIfExists = [&](PropertyIndex index) -> bool {
        const Property* prop = weather.getProperty(index);
        if (prop) {
            os << "  " << std::left << std::setw(15); // Indentation and alignment

            // Use property name directly from the Property object (e.g., "Avg Temp", "Max Wind")
            os << prop->getName() << ": ";

            // Special handling for wind direction
            if (index == WIND_DIRECTION) {
                os << degreesToCardinal(prop->getValue())
                   << " (" << std::fixed << std::setprecision(1) << prop->getValue() << "\370)"; // Using \370 for degree symbol
            } else {
                // Default formatting for numerical values
                os << std::fixed << std::setprecision(1) << prop->getValue();
                if (!prop->getUnit().empty()) { os << " " << prop->getUnit(); }
            }
            os << std::endl;
            return true;
        }
        return false;
    };

    // Iterate through the desired display order and print properties
    for(PropertyIndex index : dailyDisplayOrder) {
        if (printIfExists(index)) {
            dataDisplayed = true;
        }
    }

     // If no relevant properties were displayed at all, print a placeholder.
     if (!dataDisplayed) {
         os << "  (No specific forecast details available for this day)" << std::endl;
     }
}