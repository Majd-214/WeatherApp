// ApiConverter.cpp
#include "APIConverter.h"
#include "CurrentWeatherReport.h"
#include "ForecastReport.h"
#include "Weather.h"
#include "Forecast.h"
#include "Property.h"
#include "httplib.h" // Include needed for unique_ptr<Client> definition
#include "nlohmann/json.hpp"

// Standard library includes
#include <iostream>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <utility> // For std::move
#include <ctime>   // For time_t, tm, etc.
#include <string>  // Include string explicitly
#include <memory>  // For std::unique_ptr, std::make_unique

// Use standard namespace as requested
using namespace std;

// Use nlohmann::json namespace alias
using json = nlohmann::json;

// --- Constructor / Destructor ---
APIConverter::APIConverter(const string& apiBaseUrl) {
    client = make_unique<httplib::Client>(apiBaseUrl.c_str());
    client->set_connection_timeout(5, 0); // 5 seconds
    client->set_read_timeout(10, 0);      // 10 seconds
    units = "Metric"; // Default units
}

APIConverter::~APIConverter() = default; // Needed due to unique_ptr<httplib::Client>

// --- Configuration ---
void APIConverter::setApiKey(const string& key) { apiKey = key; }
void APIConverter::setLocation(const string& loc) { location = loc; }

bool APIConverter::setUnits(const string& unit) {
     if (unit == "Metric" || unit == "Imperial") {
        units = unit;
        return true;
    }
    return false; // Let caller handle feedback if needed
}

// --- Helper: Safe JSON Access ---
namespace { // Anonymous namespace for internal helpers
    // Helper to get a double, checking for existence and numeric type
    double getJsonDouble(const json& obj, const string& key, double defaultVal = 0.0) {
        return obj.contains(key) && obj[key].is_number() ? obj[key].get<double>() : defaultVal;
    }
    // Helper to get a string, checking for existence and string type
    string getJsonString(const json& obj, const string& key, const string& defaultVal = "N/A") {
         return obj.contains(key) && obj[key].is_string() ? obj[key].get<string>() : defaultVal;
    }
    // Helper to get long long, checking for existence and integer type
    long long getJsonLong(const json& obj, const string& key, long long defaultVal = 0) {
         return obj.contains(key) && obj[key].is_number_integer() ? obj[key].get<long long>() : defaultVal;
    }
    // Helper to get int, checking for existence and integer type
     int getJsonInt(const json& obj, const string& key, int defaultVal = 0) {
         return obj.contains(key) && obj[key].is_number_integer() ? obj[key].get<int>() : defaultVal;
    }
} // end anonymous namespace

// --- API Interaction ---

unique_ptr<CurrentWeatherReport> APIConverter::getCurrentWeather() {
    if (apiKey.empty() || location.empty()) {
        cerr << "Error: API Key or Location is not set for APIConverter." << endl;
        return nullptr;
    }
    if (!client) {
         cerr << "Error: HTTP client not initialized." << endl;
         return nullptr;
    }

    // API endpoint for current weather
    string apiUrl = "/v1/current.json?key=" + apiKey + "&q=" + location + "&aqi=no";
    httplib::Result res = client->Get(apiUrl.c_str());

    Weather currentConditions; // Weather object to populate

    if (res && res->status == 200) {
        try {
            json data = json::parse(res->body); // Parse JSON response

            // Log location info if present
            if (data.contains("location")) {
                 const auto& loc = data["location"];
                 cout << "Showing weather for: "
                      << getJsonString(loc, "name") << ", "
                      << getJsonString(loc, "region") << ", "
                      << getJsonString(loc, "country") << endl;
            }

            // Process 'current' weather data block
            if (data.contains("current")) {
                const auto& current = data["current"];
                bool isImperial = (units == "Imperial");

                // Define unit strings based on selected units (with degree symbol \370)
                string tempUnit = isImperial ? "\370F" : "\370C";
                string speedUnit = isImperial ? "mph" : "km/h";
                string precipUnit = isImperial ? "in" : "mm";
                string pressureUnit = isImperial ? "in" : "mb"; // Pressure units differ
                string visUnit = isImperial ? "miles" : "km";
                string dirUnit = "\370"; // Degree symbol for direction

                // Populate Weather object by reading directly from the correct JSON fields
                currentConditions.setProperty(TEMPERATURE, new Property("Temperature", getJsonDouble(current, isImperial ? "temp_f" : "temp_c"), tempUnit));
                currentConditions.setProperty(FEELS_LIKE, new Property("Feels Like", getJsonDouble(current, isImperial ? "feelslike_f" : "feelslike_c"), tempUnit));
                currentConditions.setProperty(WIND_SPEED, new Property("Wind Speed", getJsonDouble(current, isImperial ? "wind_mph" : "wind_kph"), speedUnit));
                currentConditions.setProperty(WIND_DIRECTION, new Property("Wind Dir", getJsonDouble(current, "wind_degree"), dirUnit)); // Degree is unitless
                currentConditions.setProperty(HUMIDITY, new Property("Humidity", getJsonDouble(current, "humidity"), "%")); // Humidity usually just %
                currentConditions.setProperty(PRESSURE, new Property("Pressure", getJsonDouble(current, isImperial ? "pressure_in" : "pressure_mb"), pressureUnit));
                currentConditions.setProperty(VISIBILITY, new Property("Visibility", getJsonDouble(current, isImperial ? "vis_miles" : "vis_km"), visUnit));
                currentConditions.setProperty(UV, new Property("UV Index", getJsonDouble(current, "uv"), "")); // UV is index
                currentConditions.setProperty(GUST_SPEED, new Property("Gust Speed", getJsonDouble(current, isImperial ? "gust_mph" : "gust_kph"), speedUnit));
                currentConditions.setProperty(PRECIPITATION, new Property("Precipitation", getJsonDouble(current, isImperial ? "precip_in" : "precip_mm"), precipUnit));
                currentConditions.setProperty(CLOUD, new Property("Cloud Cover", getJsonDouble(current, "cloud"), "%")); // Cloud cover %

                // Handle epoch time (store as double, but cast for time functions if needed later)
                long long epoch_ll = getJsonLong(current, "last_updated_epoch");
                currentConditions.setProperty(LAST_UPDATED, new Property("Last Updated", static_cast<double>(epoch_ll), "Epoch"));

                // Optionally display condition text immediately
                 if (current.contains("condition") && current["condition"].contains("text")) {
                      cout << "Condition: " << getJsonString(current["condition"], "text") << endl;
                      // Could add a CONDITION property if Weather enum is extended
                 }

            } else {
                 cerr << "Warning: 'current' data block missing in API response." << endl;
            }

        } catch (const json::exception& e) { // Catch specific JSON errors
            cerr << "JSON Error processing current weather: " << e.what() << endl;
            return nullptr; // Indicate failure by returning null pointer
        } catch (const exception& e) { // Catch other standard exceptions
            cerr << "Error processing current weather data: " << e.what() << endl;
             return nullptr;
        }
    } else { // Handle HTTP transport or status code errors
        string errorMsg = "Error fetching current weather data.";
         if (res) {
            errorMsg += " Status code: " + to_string(res->status);
             if (!res->body.empty()) { /* Add detailed error parsing if desired */ }
        } else {
            // Use httplib::to_string for httplib::Error enum
            errorMsg += " HTTP request failed (Error code: " + httplib::to_string(res.error()) + "). Check URL and network.";
        }
        cerr << errorMsg << endl;
        return nullptr; // Indicate failure
    }

    // Success: Return the populated report object
    return make_unique<CurrentWeatherReport>(move(currentConditions));
}


unique_ptr<ForecastReport> APIConverter::getForecastReport(int days, ForecastReport::DetailLevel detail) {
     if (apiKey.empty() || location.empty() ) { cerr << "Error: API Key or Location not set." << endl; return nullptr; }
     if (days < 1 || days > 14) { cerr << "Error: Invalid forecast days requested (1-14)." << endl; return nullptr; }
     if (!client) { cerr << "Error: HTTP client not initialized." << endl; return nullptr; }

    // API endpoint for forecast weather
    string apiUrl = "/v1/forecast.json?key=" + apiKey + "&q=" + location + "&days=" + to_string(days) + "&aqi=no&alerts=no";
    httplib::Result res = client->Get(apiUrl.c_str());

    Forecast forecastDataContainer; // Forecast object to populate

    if (res && res->status == 200) {
         try {
            json data = json::parse(res->body);

            if (data.contains("forecast") && data["forecast"].contains("forecastday")) {
                 bool isImperial = (units == "Imperial"); // Check units once per call

                 // Define unit strings based on selected units (with degree symbol \370)
                 string tempUnit = isImperial ? "\370F" : "\370C";
                 string speedUnit = isImperial ? "mph" : "km/h";
                 string precipUnit = isImperial ? "in" : "mm";
                 string visUnit = isImperial ? "miles" : "km";
                 string dirUnit = "\370"; // Degree symbol for direction

                 for (const auto& dayData : data["forecast"]["forecastday"]) {
                     string dateStr = getJsonString(dayData, "date", "Unknown Date");

                     // --- Daily Summary ---
                     Weather dayWeatherSummary;
                     if (dayData.contains("day")) {
                          const auto& day = dayData["day"];
                          // Populate dayWeatherSummary directly using correct fields
                          dayWeatherSummary.setProperty(TEMPERATURE, new Property("Avg Temp", getJsonDouble(day, isImperial ? "avgtemp_f" : "avgtemp_c"), tempUnit));
                          dayWeatherSummary.setProperty(WIND_SPEED, new Property("Max Wind", getJsonDouble(day, isImperial ? "maxwind_mph" : "maxwind_kph"), speedUnit));
                          dayWeatherSummary.setProperty(HUMIDITY, new Property("Avg Humidity", getJsonDouble(day, "avghumidity"), "%"));
                          dayWeatherSummary.setProperty(PRECIPITATION, new Property("Total Precip", getJsonDouble(day, isImperial ? "totalprecip_in" : "totalprecip_mm"), precipUnit));
                          dayWeatherSummary.setProperty(VISIBILITY, new Property("Avg Visibility", getJsonDouble(day, isImperial ? "avgvis_miles" : "avgvis_km"), visUnit)); // Avg visibility for day
                          dayWeatherSummary.setProperty(UV, new Property("Max UV", getJsonDouble(day, "uv"), ""));
                           // Could add daily condition text as a property if needed
                     }
                     DailyForecast dailyForecast(dateStr, move(dayWeatherSummary)); // Move summary weather

                     // --- Hourly Details ---
                      if (dayData.contains("hour")) {
                          for (const auto& hourData : dayData["hour"]) {
                             Weather hourlyWeather; // Create Weather object for this hour

                             // Populate hourlyWeather directly using correct fields
                             hourlyWeather.setProperty(TEMPERATURE, new Property("Temperature", getJsonDouble(hourData, isImperial ? "temp_f" : "temp_c"), tempUnit));
                             hourlyWeather.setProperty(FEELS_LIKE, new Property("Feels Like", getJsonDouble(hourData, isImperial ? "feelslike_f" : "feelslike_c"), tempUnit));
                             hourlyWeather.setProperty(WIND_SPEED, new Property("Wind Speed", getJsonDouble(hourData, isImperial ? "wind_mph" : "wind_kph"), speedUnit));
                             hourlyWeather.setProperty(WIND_DIRECTION, new Property("Wind Dir", getJsonDouble(hourData, "wind_degree"), dirUnit));
                             hourlyWeather.setProperty(HUMIDITY, new Property("Humidity", getJsonDouble(hourData, "humidity"), "%"));
                             hourlyWeather.setProperty(VISIBILITY, new Property("Visibility", getJsonDouble(hourData, isImperial ? "vis_miles" : "vis_km"), visUnit));
                             hourlyWeather.setProperty(GUST_SPEED, new Property("Gust Speed", getJsonDouble(hourData, isImperial ? "gust_mph" : "gust_kph"), speedUnit));
                             hourlyWeather.setProperty(PRECIPITATION, new Property("Precipitation", getJsonDouble(hourData, isImperial ? "precip_in" : "precip_mm"), precipUnit));
                             hourlyWeather.setProperty(CLOUD, new Property("Cloud Cover", getJsonDouble(hourData, "cloud"), "%"));
                             // Add Feels Like, Pressure, etc. if needed from hourly data
                             hourlyWeather.setProperty(PRESSURE, new Property("Pressure", getJsonDouble(hourData, isImperial ? "pressure_in" : "pressure_mb"), isImperial ? "in" : "mb"));


                             // Get time string from epoch
                             long long epochTimeLL = getJsonLong(hourData, "time_epoch");
                             time_t epochTime = static_cast<time_t>(epochTimeLL);
                             tm timeinfo = {}; // Use tm, not std::tm
                             #ifdef _WIN32
                                 localtime_s(&timeinfo, &epochTime);
                             #else
                                 localtime_r(&epochTime, &timeinfo);
                             #endif
                             stringstream timeStream;
                             timeStream << put_time(&timeinfo, "%H:%M"); // HH:MM format
                             string timeStr = timeStream.str();

                             // Add the populated hourly forecast, moving the weather data
                             dailyForecast.addHourlyForecast(HourlyForecast(move(hourlyWeather), timeStr));
                          }
                     }
                     // Add the fully populated daily forecast (with hourly data if processed)
                     forecastDataContainer.addDailyForecast(move(dailyForecast));
                 }
             } else {
                 cerr << "Warning: 'forecast'/'forecastday' data block missing in API response." << endl;
             }

         } catch (const json::exception& e) {
             cerr << "JSON Error processing forecast: " << e.what() << endl;
             return nullptr; // Indicate failure
         } catch (const exception& e) {
              cerr << "Error processing forecast data: " << e.what() << endl;
             return nullptr; // Indicate failure
         }
    } else { // Handle HTTP errors
        string errorMsg = "Error fetching forecast data.";
         if (res) {
            errorMsg += " Status code: " + to_string(res->status);
              if (!res->body.empty()) { /* Add detailed error parsing if desired */ }
        } else { errorMsg += " HTTP request failed (Error code: " + httplib::to_string(res.error()) + ")."; }
        cerr << errorMsg << endl;
        return nullptr; // Indicate failure
    }

    // Success: Return the populated report object
    return make_unique<ForecastReport>(move(forecastDataContainer), detail);
}