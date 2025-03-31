// ApiConverter.cpp
#include "APIConverter.h"
#include "nlohmann/json.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <vector>
#include <stdexcept> // For exception handling with json

using json = nlohmann::json;

// Default constructor - maybe initialize with empty key/loc?
APIConverter::APIConverter() : client("http://api.weatherapi.com"), units("Metric") {}

// Constructor with key and location
APIConverter::APIConverter(const std::string& key, const std::string& loc)
    : client("http://api.weatherapi.com"), apiKey(key), location(loc), units("Metric") {} // Initialize members directly

std::string APIConverter::getApiKey() const {
    return apiKey;
}

std::string APIConverter::getLocation() const {
    return location;
}

// Returns the currently set units ("Metric" or "Imperial")
std::string APIConverter::getUnits() const {
     return units;
}

void APIConverter::setUnits(const std::string& unit) {
    if (unit == "Metric" || unit == "Imperial") {
        units = unit;
    } else {
        std::cerr << "Warning: Invalid unit '" << unit << "' provided to setUnits. Keeping '" << units << "'." << std::endl;
    }
}

void APIConverter::setApiKey(const std::string& key) {
    apiKey = key;
}

void APIConverter::setLocation(const std::string& loc) {
    location = loc;
}

void APIConverter::updateWeather(Weather* weather) {
    if (apiKey.empty() || location.empty()) {
        std::cerr << "Error: API Key or Location is not set." << std::endl;
        return;
    }
    // Construct API URL
    // Note: WeatherAPI might support a 'units=m' or 'units=f' parameter,
    // but here we fetch metric and convert based on 'this->units' setting.
    std::string apiUrl = "/v1/current.json?key=" + apiKey + "&q=" + location + "&aqi=no"; // Added no aqi
    auto res = client.Get(apiUrl.c_str());

    if (res && res->status == 200) {
        try {
            json weatherData = json::parse(res->body); // Use default parse

            if (weatherData.contains("location")) {
                 const auto& loc = weatherData["location"];
                 std::cout << "Showing weather for: "
                           << loc.value("name", "N/A") << ", "
                           << loc.value("region", "N/A") << ", "
                           << loc.value("country", "N/A") << std::endl;
            }


            if (weatherData.contains("current")) {
                const auto& current = weatherData["current"];
                // Helper lambda for safe JSON access
                auto getDouble = [&](const json& obj, const std::string& key) {
                    return obj.contains(key) && !obj[key].is_null() ? obj[key].get<double>() : 0.0; // Default to 0.0 if missing/null
                };
                 auto getString = [&](const json& obj, const std::string& key) {
                    return obj.contains(key) && obj[key].is_string() ? obj[key].get<std::string>() : "N/A";
                };


                // Use metric values from API and determine units/values based on 'this->units'
                double temp_api = getDouble(current, "temp_c");
                double feels_api = getDouble(current, "feelslike_c");
                double wind_kph_api = getDouble(current, "wind_kph");
                double gust_kph_api = getDouble(current, "gust_kph");
                double precip_mm_api = getDouble(current, "precip_mm");
                double vis_km_api = getDouble(current, "vis_km");

                double temp_val = (units == "Imperial") ? (temp_api * 9.0/5.0 + 32.0) : temp_api;
                double feels_val = (units == "Imperial") ? (feels_api * 9.0/5.0 + 32.0) : feels_api;
                double wind_val = (units == "Imperial") ? (wind_kph_api * 0.621371) : wind_kph_api; // kph to mph
                double gust_val = (units == "Imperial") ? (gust_kph_api * 0.621371) : gust_kph_api; // kph to mph
                double precip_val = (units == "Imperial") ? (precip_mm_api * 0.0393701) : precip_mm_api; // mm to inches
                double vis_val = (units == "Imperial") ? (vis_km_api * 0.621371) : vis_km_api; // km to miles

                std::string temp_unit = (units == "Imperial") ? "\370F" : "\370C"; // Or use °F / °C
                std::string speed_unit = (units == "Imperial") ? "mph" : "km/h";
                std::string precip_unit = (units == "Imperial") ? "in" : "mm";
                std::string vis_unit = (units == "Imperial") ? "miles" : "km";


                weather->setProperty(LAST_UPDATED, new Property("Last Updated", getDouble(current, "last_updated_epoch"), "Epoch")); // Keep epoch as is
                weather->setProperty(TEMPERATURE, new Property("Temperature", temp_val, temp_unit));
                weather->setProperty(FEELS_LIKE, new Property("Feels Like", feels_val, temp_unit));
                weather->setProperty(WIND_SPEED, new Property("Wind Speed", wind_val, speed_unit));
                weather->setProperty(WIND_DIRECTION, new Property("Wind Dir", getDouble(current, "wind_degree"), "\370")); // Use ° symbol
                weather->setProperty(GUST_SPEED, new Property("Gust Speed", gust_val, speed_unit));
                weather->setProperty(HUMIDITY, new Property("Humidity", getDouble(current, "humidity"), "%"));
                weather->setProperty(CLOUD, new Property("Cloud Cover", getDouble(current, "cloud"), "%"));
                // Pressure: API provides mb and in. Let's stick to mb for now unless units required.
                weather->setProperty(PRESSURE, new Property("Pressure", getDouble(current, "pressure_mb"), "mb"));
                weather->setProperty(VISIBILITY, new Property("Visibility", vis_val, vis_unit));
                weather->setProperty(UV, new Property("UV Index", getDouble(current, "uv"), ""));
                weather->setProperty(PRECIPITATION, new Property("Precipitation", precip_val, precip_unit));

                // Add condition text
                 if (current.contains("condition") && current["condition"].contains("text")) {
                      // Maybe add a 'CONDITION' PropertyIndex? For now, just print it.
                      std::cout << "Condition: " << current["condition"]["text"].get<std::string>() << std::endl;
                 }


            } else {
                 std::cerr << "Error: 'current' data missing in API response." << std::endl;
            }

        } catch (const json::parse_error& e) {
            std::cerr << "JSON parse error in APIConverter (updateWeather): " << e.what() << std::endl;
        } catch (const json::type_error& e) {
             std::cerr << "JSON type error in APIConverter (updateWeather): " << e.what() << std::endl;
        } catch (const std::exception& e) {
             std::cerr << "Generic error processing weather data: " << e.what() << std::endl;
        }
    } else {
        // Handle API errors more robustly
        std::string errorMsg = "Error fetching current weather data.";
        if (res) {
            errorMsg += " Status code: " + std::to_string(res->status);
            if (!res->body.empty()) {
                try {
                    json errorJson = json::parse(res->body);
                    if (errorJson.contains("error") && errorJson["error"].contains("message")) {
                        errorMsg += ". Message: " + errorJson["error"]["message"].get<std::string>();
                    } else {
                         errorMsg += ". Response body: " + res->body;
                    }
                } catch(...) { // Ignore parse errors in error body
                    errorMsg += ". Couldn't parse error response body: " + res->body;
                }
            }
        } else {
            errorMsg += " HTTP request failed (no response). Check network connection and httplib setup.";
            // Log the specific error from httplib if possible, e.g., httplib::Error err = res.error();
        }
        std::cerr << errorMsg << std::endl;
    }
}


Forecast* APIConverter::getForecast(int days) {
     if (apiKey.empty() || location.empty()) {
        std::cerr << "Error: API Key or Location is not set." << std::endl;
        return nullptr;
    }
     if (days < 1 || days > 14) { // WeatherAPI typically supports up to 14 days
         std::cerr << "Error: Invalid number of forecast days requested (" << days << "). Must be 1-14." << std::endl;
         return nullptr;
     }

    std::string apiUrl = "/v1/forecast.json?key=" + apiKey + "&q=" + location + "&days=" + std::to_string(days) + "&aqi=no&alerts=no";
    auto res = client.Get(apiUrl.c_str());
    auto forecast = new Forecast(); // Allocate forecast object

    if (res && res->status == 200) {
        try {
            json forecastData = json::parse(res->body);

            if (forecastData.contains("forecast") && forecastData["forecast"].contains("forecastday")) {
                for (const auto& dayData : forecastData["forecast"]["forecastday"]) {
                    std::string dateStr = dayData.value("date", "Unknown Date");

                    // Helper lambda for safe JSON access within the loop
                    auto getDouble = [&](const json& obj, const std::string& key) {
                         return obj.contains(key) && !obj[key].is_null() ? obj[key].get<double>() : 0.0;
                    };

                    // --- Daily Summary ---
                    Weather dayWeather;
                    if (dayData.contains("day")) {
                        const auto& day = dayData["day"];
                        // Fetch metric values and convert based on 'units'
                        double avgtemp_c_api = getDouble(day, "avgtemp_c");
                        double maxwind_kph_api = getDouble(day, "maxwind_kph");
                        double totalprecip_mm_api = getDouble(day, "totalprecip_mm");

                        double temp_val = (units == "Imperial") ? (avgtemp_c_api * 9.0/5.0 + 32.0) : avgtemp_c_api;
                        double wind_val = (units == "Imperial") ? (maxwind_kph_api * 0.621371) : maxwind_kph_api;
                        double precip_val = (units == "Imperial") ? (totalprecip_mm_api * 0.0393701) : totalprecip_mm_api;


                        std::string temp_unit = (units == "Imperial") ? "\370F" : "\370C";
                        std::string speed_unit = (units == "Imperial") ? "mph" : "km/h";
                         std::string precip_unit = (units == "Imperial") ? "in" : "mm";


                        dayWeather.setProperty(TEMPERATURE, new Property("Avg Temp", temp_val, temp_unit));
                        dayWeather.setProperty(WIND_SPEED, new Property("Max Wind", wind_val, speed_unit));
                        dayWeather.setProperty(HUMIDITY, new Property("Avg Humidity", getDouble(day, "avghumidity"), "%"));
                        dayWeather.setProperty(PRECIPITATION, new Property("Total Precip", precip_val, precip_unit));
                        dayWeather.setProperty(UV, new Property("Max UV", getDouble(day, "uv"), "")); // Daily UV
                         // Add daily condition text?
                         if (day.contains("condition") && day["condition"].contains("text")) {
                             // Maybe add a 'CONDITION' PropertyIndex?
                         }
                    }
                    DailyForecast dailyForecast(dateStr, dayWeather); // Pass constructed daily weather

                    // --- Hourly Details ---
                    if (dayData.contains("hour")) {
                        for (const auto& hourData : dayData["hour"]) {
                            Weather hourlyWeather;
                            std::time_t epochTime = static_cast<std::time_t>(getDouble(hourData, "time_epoch"));
                            std::tm timeinfo = {}; // Initialize to zero
                            #ifdef _WIN32
                                localtime_s(&timeinfo, &epochTime);
                            #else
                                localtime_r(&epochTime, &timeinfo); // Use localtime_r for thread-safety on POSIX
                            #endif
                            std::stringstream timeStream;
                            timeStream << std::put_time(&timeinfo, "%H:%M"); // HH:MM format
                            std::string timeStr = timeStream.str();

                             // Fetch metric values and convert based on 'units'
                            double temp_c_api = getDouble(hourData, "temp_c");
                            double feelslike_c_api = getDouble(hourData, "feelslike_c");
                            double wind_kph_api = getDouble(hourData, "wind_kph");
                             double precip_mm_api = getDouble(hourData, "precip_mm");
                              double vis_km_api = getDouble(hourData, "vis_km");

                            double temp_val = (units == "Imperial") ? (temp_c_api * 9.0/5.0 + 32.0) : temp_c_api;
                            double feels_val = (units == "Imperial") ? (feelslike_c_api * 9.0/5.0 + 32.0) : feelslike_c_api;
                            double wind_val = (units == "Imperial") ? (wind_kph_api * 0.621371) : wind_kph_api;
                             double precip_val = (units == "Imperial") ? (precip_mm_api * 0.0393701) : precip_mm_api; // mm to inches
                            double vis_val = (units == "Imperial") ? (vis_km_api * 0.621371) : vis_km_api; // km to miles


                            std::string temp_unit = (units == "Imperial") ? "\370F" : "\370C";
                            std::string speed_unit = (units == "Imperial") ? "mph" : "km/h";
                             std::string precip_unit = (units == "Imperial") ? "in" : "mm";
                             std::string vis_unit = (units == "Imperial") ? "miles" : "km";


                            hourlyWeather.setProperty(TEMPERATURE, new Property("Temperature", temp_val, temp_unit));
                            hourlyWeather.setProperty(FEELS_LIKE, new Property("Feels Like", feels_val, temp_unit));
                            hourlyWeather.setProperty(WIND_SPEED, new Property("Wind Speed", wind_val, speed_unit));
                            hourlyWeather.setProperty(WIND_DIRECTION, new Property("Wind Dir", getDouble(hourData, "wind_degree"), "\370"));
                            hourlyWeather.setProperty(HUMIDITY, new Property("Humidity", getDouble(hourData, "humidity"), "%"));
                            hourlyWeather.setProperty(CLOUD, new Property("Cloud Cover", getDouble(hourData, "cloud"), "%"));
                            hourlyWeather.setProperty(PRECIPITATION, new Property("Precipitation", precip_val, precip_unit)); // Hourly precip
                             hourlyWeather.setProperty(VISIBILITY, new Property("Visibility", vis_val, vis_unit)); // Hourly vis
                             hourlyWeather.setProperty(GUST_SPEED, new Property("Wind Gust", (units == "Imperial" ? getDouble(hourData,"gust_mph") : getDouble(hourData,"gust_kph")), speed_unit)); // Hourly gust

                             // Add hourly condition text?
                            if (hourData.contains("condition") && hourData["condition"].contains("text")) {
                                // Maybe add a 'CONDITION' PropertyIndex?
                            }


                            dailyForecast.addHourlyForecast(HourlyForecast(hourlyWeather, timeStr)); // Pass constructed hourly weather
                        }
                    }
                    forecast->addDailyForecast(dailyForecast);
                }
            } else {
                 std::cerr << "Error: 'forecast' or 'forecastday' data missing in API response." << std::endl;
                 // Don't delete forecast here, let it return partially filled or empty if necessary
            }

        } catch (const json::parse_error& e) {
            std::cerr << "JSON parse error in APIConverter (getForecast): " << e.what() << std::endl;
            delete forecast; // Clean up allocated memory on error
            return nullptr;
        } catch (const json::type_error& e) {
             std::cerr << "JSON type error in APIConverter (getForecast): " << e.what() << std::endl;
             delete forecast;
             return nullptr;
        } catch (const std::exception& e) {
             std::cerr << "Generic error processing forecast data: " << e.what() << std::endl;
             delete forecast;
             return nullptr;
        }
    } else {
         // Handle API errors more robustly
        std::string errorMsg = "Error fetching forecast data.";
         if (res) {
            errorMsg += " Status code: " + std::to_string(res->status);
             if (!res->body.empty()) {
                try {
                    json errorJson = json::parse(res->body);
                    if (errorJson.contains("error") && errorJson["error"].contains("message")) {
                        errorMsg += ". Message: " + errorJson["error"]["message"].get<std::string>();
                    } else {
                         errorMsg += ". Response body: " + res->body;
                    }
                } catch(...) { // Ignore parse errors in error body
                    errorMsg += ". Couldn't parse error response body: " + res->body;
                }
            }
        } else {
            errorMsg += " HTTP request failed (no response). Check network connection and httplib setup.";
        }
        std::cerr << errorMsg << std::endl;
        delete forecast; // Clean up allocated memory on error
        return nullptr;
    }
    return forecast; // Return the populated forecast object
}