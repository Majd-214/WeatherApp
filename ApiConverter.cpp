// ApiConverter.cpp
#include "APIConverter.h"
#include "CurrentWeatherReport.h" // Definition for unique_ptr creation
#include "ForecastReport.h"       // Definition for unique_ptr creation
#include "Weather.h"              // Definition for Weather data structure
#include "Forecast.h"             // Definition for Forecast data structure
#include "Property.h"             // Definition for Property data structure
#include "httplib.h"              // External HTTP library
#include "nlohmann/json.hpp"      // External JSON library

#include <iostream>     // For error output (cerr)
#include <sstream>      // For string manipulation (time formatting)
#include <iomanip>      // For stream manipulators (put_time, setprecision)
#include <stdexcept>    // For exception handling (json::exception)
#include <utility>      // For std::move
#include <ctime>        // For time conversions (epoch)
#include <string>       // For std::string usage
#include <memory>       // For std::unique_ptr, std::make_unique

// Use standard namespace for convenience.
using namespace std;
// Use nlohmann::json namespace alias.
using json = nlohmann::json;

// --- Constructor / Destructor ---

// Initializes the HTTP client with the base URL and sets timeouts.
APIConverter::APIConverter(const string& apiBaseUrl) {
    client = make_unique<httplib::Client>(apiBaseUrl.c_str());
    client->set_connection_timeout(5, 0); // 5 seconds connection timeout
    client->set_read_timeout(10, 0);      // 10 seconds read timeout
    units = "Metric"; // Default to Metric units
}

// Default destructor handles unique_ptr<httplib::Client> cleanup.
APIConverter::~APIConverter() = default;

// --- Configuration ---

// Stores the provided API key.
void APIConverter::setApiKey(const string& key) { apiKey = key; }
// Stores the provided location identifier.
void APIConverter::setLocation(const string& loc) { location = loc; }

// Sets the units if valid ("Metric" or "Imperial"), returns success status.
bool APIConverter::setUnits(const string& unit) {
     if (unit == "Metric" || unit == "Imperial") {
        units = unit;
        return true;
    }
    return false; // Indicate invalid unit provided
}

// --- Helper: Safe JSON Access ---
namespace { // Anonymous namespace for internal linkage helpers

    // Safely extracts a double value from a JSON object.
    double getJsonDouble(const json& obj, const string& key, double defaultVal = 0.0) {
        return obj.contains(key) && obj[key].is_number() ? obj[key].get<double>() : defaultVal;
    }
    // Safely extracts a string value from a JSON object.
    string getJsonString(const json& obj, const string& key, const string& defaultVal = "N/A") {
         return obj.contains(key) && obj[key].is_string() ? obj[key].get<string>() : defaultVal;
    }
    // Safely extracts a long long integer value from a JSON object.
    long long getJsonLong(const json& obj, const string& key, long long defaultVal = 0) {
         return obj.contains(key) && obj[key].is_number_integer() ? obj[key].get<long long>() : defaultVal;
    }
    // Safely extracts an integer value from a JSON object.
     int getJsonInt(const json& obj, const string& key, int defaultVal = 0) {
         return obj.contains(key) && obj[key].is_number_integer() ? obj[key].get<int>() : defaultVal;
    }
} // end anonymous namespace

// --- API Interaction ---

// Fetches and parses the current weather data from the API.
unique_ptr<CurrentWeatherReport> APIConverter::getCurrentWeather() {
    // Pre-flight checks for necessary configuration.
    if (apiKey.empty() || location.empty()) {
        cerr << "Error: API Key or Location is not set for APIConverter." << endl;
        return nullptr;
    }
    if (!client) {
         cerr << "Error: HTTP client not initialized." << endl;
         return nullptr;
    }

    // Construct the API request URL.
    string apiUrl = "/v1/current.json?key=" + apiKey + "&q=" + location + "&aqi=no";
    // Perform the GET request.
    httplib::Result res = client->Get(apiUrl.c_str());

    Weather currentConditions; // Weather object to hold parsed data.

    // Check for successful HTTP response.
    if (res && res->status == 200) {
        try {
            json data = json::parse(res->body); // Parse the JSON response body.

            // Display location information if available.
            if (data.contains("location")) {
                 const auto& loc = data["location"];
                 cout << "Showing weather for: "
                      << getJsonString(loc, "name") << ", "
                      << getJsonString(loc, "region") << ", "
                      << getJsonString(loc, "country") << endl;
            }

            // Process the 'current' weather data block.
            if (data.contains("current")) {
                const auto& current = data["current"];
                bool isImperial = (units == "Imperial");

                // Determine unit strings based on the selected unit system.
                string tempUnit = isImperial ? "\370F" : "\370C";   // Degree symbol: \370
                string speedUnit = isImperial ? "mph" : "km/h";
                string precipUnit = isImperial ? "in" : "mm";
                string pressureUnit = isImperial ? "in" : "mb";
                string visUnit = isImperial ? "miles" : "km";
                string dirUnit = "\370"; // Degree symbol for wind direction

                // Populate the Weather object using safe JSON helpers.
                // Dynamically allocates Property objects.
                currentConditions.setProperty(TEMPERATURE, new Property("Temperature", getJsonDouble(current, isImperial ? "temp_f" : "temp_c"), tempUnit));
                currentConditions.setProperty(FEELS_LIKE, new Property("Feels Like", getJsonDouble(current, isImperial ? "feelslike_f" : "feelslike_c"), tempUnit));
                currentConditions.setProperty(WIND_SPEED, new Property("Wind Speed", getJsonDouble(current, isImperial ? "wind_mph" : "wind_kph"), speedUnit));
                currentConditions.setProperty(WIND_DIRECTION, new Property("Wind Dir", getJsonDouble(current, "wind_degree"), dirUnit));
                currentConditions.setProperty(HUMIDITY, new Property("Humidity", getJsonDouble(current, "humidity"), "%"));
                currentConditions.setProperty(PRESSURE, new Property("Pressure", getJsonDouble(current, isImperial ? "pressure_in" : "pressure_mb"), pressureUnit));
                currentConditions.setProperty(VISIBILITY, new Property("Visibility", getJsonDouble(current, isImperial ? "vis_miles" : "vis_km"), visUnit));
                currentConditions.setProperty(UV, new Property("UV Index", getJsonDouble(current, "uv"), ""));
                currentConditions.setProperty(GUST_SPEED, new Property("Gust Speed", getJsonDouble(current, isImperial ? "gust_mph" : "gust_kph"), speedUnit));
                currentConditions.setProperty(PRECIPITATION, new Property("Precipitation", getJsonDouble(current, isImperial ? "precip_in" : "precip_mm"), precipUnit));
                currentConditions.setProperty(CLOUD, new Property("Cloud Cover", getJsonDouble(current, "cloud"), "%"));

                // Store epoch time as a double value in a Property.
                long long epoch_ll = getJsonLong(current, "last_updated_epoch");
                currentConditions.setProperty(LAST_UPDATED, new Property("Last Updated", static_cast<double>(epoch_ll), "Epoch"));

                // Optionally log the text condition description.
                 if (current.contains("condition") && current["condition"].contains("text")) {
                      cout << "Condition: " << getJsonString(current["condition"], "text") << endl;
                 }

            } else {
                 cerr << "Warning: 'current' data block missing in API response." << endl;
                 // Proceed without current data, report might be empty.
            }

        } catch (const json::exception& e) { // Handle JSON parsing errors.
            cerr << "JSON Error processing current weather: " << e.what() << endl;
            return nullptr; // Indicate failure.
        } catch (const exception& e) { // Handle other potential errors during processing.
            cerr << "Error processing current weather data: " << e.what() << endl;
             return nullptr;
        }
    } else { // Handle HTTP request errors (network issue, bad status code).
        string errorMsg = "Error fetching current weather data.";
         if (res) { // If response object exists, include status code.
            errorMsg += " Status code: " + to_string(res->status);
        } else { // If no response object, use httplib error code.
            errorMsg += " HTTP request failed (Error code: " + httplib::to_string(res.error()) + "). Check URL and network.";
        }
        cerr << errorMsg << endl;
        return nullptr; // Indicate failure.
    }

    // If successful, create and return the report object, transferring ownership of Weather data.
    return make_unique<CurrentWeatherReport>(move(currentConditions));
}


// Fetches and parses forecast weather data from the API.
unique_ptr<ForecastReport> APIConverter::getForecastReport(int days, ForecastReport::DetailLevel detail) {
     // Pre-flight checks.
     if (apiKey.empty() || location.empty() ) { cerr << "Error: API Key or Location not set." << endl; return nullptr; }
     if (days < 1 || days > 14) { cerr << "Error: Invalid forecast days requested (1-14)." << endl; return nullptr; } // WeatherAPI limit
     if (!client) { cerr << "Error: HTTP client not initialized." << endl; return nullptr; }

    // Construct forecast API request URL.
    string apiUrl = "/v1/forecast.json?key=" + apiKey + "&q=" + location + "&days=" + to_string(days) + "&aqi=no&alerts=no";
    // Perform GET request.
    httplib::Result res = client->Get(apiUrl.c_str());

    Forecast forecastDataContainer; // Forecast object to hold parsed data.

    // Check for successful HTTP response.
    if (res && res->status == 200) {
         try {
            json data = json::parse(res->body); // Parse JSON response.

            // Check for the main forecast data array.
            if (data.contains("forecast") && data["forecast"].contains("forecastday")) {
                 bool isImperial = (units == "Imperial"); // Check units once.

                 // Determine unit strings based on the selected unit system.
                 string tempUnit = isImperial ? "\370F" : "\370C";
                 string speedUnit = isImperial ? "mph" : "km/h";
                 string precipUnit = isImperial ? "in" : "mm";
                 string visUnit = isImperial ? "miles" : "km";
                 string dirUnit = "\370"; // Degree symbol for wind direction
                 string pressureUnit = isImperial ? "in" : "mb";

                 // Iterate through each day in the forecast array.
                 for (const auto& dayData : data["forecast"]["forecastday"]) {
                     string dateStr = getJsonString(dayData, "date", "Unknown Date");

                     // --- Process Daily Summary ---
                     Weather dayWeatherSummary; // Weather object for the day's summary.
                     if (dayData.contains("day")) {
                          const auto& day = dayData["day"];
                          // Populate daily summary Weather object.
                          dayWeatherSummary.setProperty(TEMPERATURE, new Property("Avg Temp", getJsonDouble(day, isImperial ? "avgtemp_f" : "avgtemp_c"), tempUnit));
                          dayWeatherSummary.setProperty(WIND_SPEED, new Property("Max Wind", getJsonDouble(day, isImperial ? "maxwind_mph" : "maxwind_kph"), speedUnit));
                          dayWeatherSummary.setProperty(HUMIDITY, new Property("Avg Humidity", getJsonDouble(day, "avghumidity"), "%"));
                          dayWeatherSummary.setProperty(PRECIPITATION, new Property("Total Precip", getJsonDouble(day, isImperial ? "totalprecip_in" : "totalprecip_mm"), precipUnit));
                          dayWeatherSummary.setProperty(VISIBILITY, new Property("Avg Visibility", getJsonDouble(day, isImperial ? "avgvis_miles" : "avgvis_km"), visUnit));
                          dayWeatherSummary.setProperty(UV, new Property("Max UV", getJsonDouble(day, "uv"), ""));
                     }
                     // Create DailyForecast object, transferring ownership of summary weather data.
                     DailyForecast dailyForecast(dateStr, move(dayWeatherSummary));

                     // --- Process Hourly Details ---
                      if (dayData.contains("hour")) {
                          // Iterate through each hour's data for the current day.
                          for (const auto& hourData : dayData["hour"]) {
                             Weather hourlyWeather; // Weather object for this specific hour.

                             // Populate hourly Weather object.
                             hourlyWeather.setProperty(TEMPERATURE, new Property("Temperature", getJsonDouble(hourData, isImperial ? "temp_f" : "temp_c"), tempUnit));
                             hourlyWeather.setProperty(FEELS_LIKE, new Property("Feels Like", getJsonDouble(hourData, isImperial ? "feelslike_f" : "feelslike_c"), tempUnit));
                             hourlyWeather.setProperty(WIND_SPEED, new Property("Wind Speed", getJsonDouble(hourData, isImperial ? "wind_mph" : "wind_kph"), speedUnit));
                             hourlyWeather.setProperty(WIND_DIRECTION, new Property("Wind Dir", getJsonDouble(hourData, "wind_degree"), dirUnit));
                             hourlyWeather.setProperty(HUMIDITY, new Property("Humidity", getJsonDouble(hourData, "humidity"), "%"));
                             hourlyWeather.setProperty(VISIBILITY, new Property("Visibility", getJsonDouble(hourData, isImperial ? "vis_miles" : "vis_km"), visUnit));
                             hourlyWeather.setProperty(GUST_SPEED, new Property("Gust Speed", getJsonDouble(hourData, isImperial ? "gust_mph" : "gust_kph"), speedUnit));
                             hourlyWeather.setProperty(PRECIPITATION, new Property("Precipitation", getJsonDouble(hourData, isImperial ? "precip_in" : "precip_mm"), precipUnit));
                             hourlyWeather.setProperty(CLOUD, new Property("Cloud Cover", getJsonDouble(hourData, "cloud"), "%"));
                             hourlyWeather.setProperty(PRESSURE, new Property("Pressure", getJsonDouble(hourData, isImperial ? "pressure_in" : "pressure_mb"), pressureUnit));

                             // Convert epoch time to HH:MM string format.
                             long long epochTimeLL = getJsonLong(hourData, "time_epoch");
                             time_t epochTime = static_cast<time_t>(epochTimeLL);
                             tm timeinfo = {};
                             #ifdef _WIN32 // Platform-specific safe time conversion.
                                 localtime_s(&timeinfo, &epochTime);
                             #else
                                 localtime_r(&epochTime, &timeinfo); // POSIX version
                             #endif
                             stringstream timeStream;
                             timeStream << put_time(&timeinfo, "%H:%M"); // Format as HH:MM.
                             string timeStr = timeStream.str();

                             // Add the hourly forecast data to the current day's forecast.
                             // Transfers ownership of hourlyWeather data via move.
                             dailyForecast.addHourlyForecast(HourlyForecast(move(hourlyWeather), timeStr));
                          }
                     }
                     // Add the completed daily forecast (with hourly data) to the main container.
                     // Transfers ownership of dailyForecast data via move.
                     forecastDataContainer.addDailyForecast(move(dailyForecast));
                 }
             } else {
                 cerr << "Warning: 'forecast'/'forecastday' data block missing in API response." << endl;
             }

         } catch (const json::exception& e) { // Handle JSON parsing errors.
             cerr << "JSON Error processing forecast: " << e.what() << endl;
             return nullptr;
         } catch (const exception& e) { // Handle other processing errors.
              cerr << "Error processing forecast data: " << e.what() << endl;
             return nullptr;
         }
    } else { // Handle HTTP request errors.
        string errorMsg = "Error fetching forecast data.";
         if (res) {
            errorMsg += " Status code: " + to_string(res->status);
        } else { errorMsg += " HTTP request failed (Error code: " + httplib::to_string(res.error()) + ")."; }
        cerr << errorMsg << endl;
        return nullptr;
    }

    // If successful, create and return the forecast report object.
    // Transfers ownership of forecastDataContainer via move.
    return make_unique<ForecastReport>(move(forecastDataContainer), detail);
}