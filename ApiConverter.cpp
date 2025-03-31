// APIConverter.cpp
#include "APIConverter.h"
#include "nlohmann/json.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>

using json = nlohmann::json;

APIConverter::APIConverter() : client("http://api.weatherapi.com") {}

APIConverter::APIConverter(const std::string& key, const std::string& loc)
    : client("http://api.weatherapi.com"), apiKey(key), location(loc) {}

std::string APIConverter::getApiKey() const {
    return apiKey;
}

std::string APIConverter::getLocation() const {
    return location;
}

void APIConverter::setApiKey(const std::string& key) {
    apiKey = key;
}

void APIConverter::setLocation(const std::string& loc) {
    location = loc;
}

void APIConverter::updateWeather(Weather* weather) {
    std::string apiUrl = "/v1/current.json?key=" + apiKey + "&q=" + location;
    auto res = client.Get(apiUrl.c_str());

    if (res && res->status == 200) {
        try {
            json weatherData = json::parse(res->body.c_str(), res->body.c_str() + res->body.length());

            if (weatherData.contains("current")) {
                const auto& current = weatherData["current"];
                weather->setProperty(LAST_UPDATED, new Property("Last Updated", current["last_updated_epoch"].get<double>(), "Epoch"));
                weather->setProperty(TEMPERATURE, new Property("Temperature", current["temp_c"].get<double>(), "\370C"));
                weather->setProperty(FEELS_LIKE, new Property("Feels Like", current["feelslike_c"].get<double>(), "\370C"));
                weather->setProperty(WIND_SPEED, new Property("Wind Speed", current["wind_kph"].get<double>(), "km/h"));
                weather->setProperty(WIND_DIRECTION, new Property("Wind Direction", current["wind_degree"].get<double>(), "\370"));
                weather->setProperty(GUST_SPEED, new Property("Gust Speed", current["gust_kph"].get<double>(), "km/h"));
                weather->setProperty(HUMIDITY, new Property("Humidity", current["humidity"].get<double>(), "%"));
                weather->setProperty(CLOUD, new Property("Cloud Cover", current["cloud"].get<double>(), "%"));
                weather->setProperty(PRESSURE, new Property("Pressure", current["pressure_mb"].get<double>(), "mb"));
                weather->setProperty(VISIBILITY, new Property("Visibility", current["vis_km"].get<double>(), "km"));
                weather->setProperty(UV, new Property("UV Index", current["uv"].get<double>(), ""));
                weather->setProperty(PRECIPITATION, new Property("Precipitation", current["precip_mm"].get<double>(), "mm"));
            }
            if (weatherData.contains("location")) {
                std::cout << "Location: " << weatherData["location"]["name"].get<std::string>() << ", " << weatherData["location"]["region"].get<std::string>() << ", " << weatherData["location"]["country"].get<std::string>() << std::endl;
            }

        } catch (const json::parse_error& e) {
            std::cerr << "JSON parse error in APIConverter (updateWeather): " << e.what() << std::endl;
        }
    } else {
        std::cerr << "Error fetching current weather data. Status code: " << (res ? std::to_string(res->status) : "N/A") << std::endl;
    }
}

Forecast* APIConverter::getForecast(int days) {
    std::string apiUrl = "/v1/forecast.json?key=" + apiKey + "&q=" + location + "&days=" + std::to_string(days);
    auto res = client.Get(apiUrl.c_str());
    Forecast* forecast = nullptr;

    if (res && res->status == 200) {
        try {
            json forecastData = json::parse(res->body.c_str(), res->body.c_str() + res->body.length());
            if (forecastData.contains("forecast") && forecastData["forecast"].contains("forecastday")) {
                int numDays = forecastData["forecast"]["forecastday"].size();
                forecast = new Forecast(numDays);
                for (int i = 0; i < numDays; ++i) {
                    const auto& dayData = forecastData["forecast"]["forecastday"][i];
                    std::string date = dayData["date"].get<std::string>();
                    int numHours = dayData["hour"].size();
                    ForecastDay* forecastDay = new ForecastDay(date, numHours);
                    for (int j = 0; j < numHours; ++j) {
                        const auto& hourData = dayData["hour"][j];
                        Weather hourlyWeather;
                        hourlyWeather.setProperty(LAST_UPDATED, new Property("Time", static_cast<double>(hourData["time_epoch"].get<int>()), "Epoch"));
                        hourlyWeather.setProperty(TEMPERATURE, new Property("Temperature", hourData["temp_c"].get<double>(), "\370C"));
                        hourlyWeather.setProperty(FEELS_LIKE, new Property("Feels Like", hourData["feelslike_c"].get<double>(), "\370C"));
                        hourlyWeather.setProperty(WIND_SPEED, new Property("Wind Speed", hourData["wind_kph"].get<double>(), "km/h"));
                        hourlyWeather.setProperty(WIND_DIRECTION, new Property("Wind Direction", hourData["wind_degree"].get<double>(), "\370"));
                        hourlyWeather.setProperty(GUST_SPEED, new Property("Gust Speed", hourData["gust_kph"].get<double>(), "km/h"));
                        hourlyWeather.setProperty(HUMIDITY, new Property("Humidity", hourData["humidity"].get<double>(), "%"));
                        hourlyWeather.setProperty(CLOUD, new Property("Cloud Cover", hourData["cloud"].get<double>(), "%"));
                        hourlyWeather.setProperty(PRESSURE, new Property("Pressure", hourData["pressure_mb"].get<double>(), "mb"));
                        hourlyWeather.setProperty(VISIBILITY, new Property("Visibility", hourData["vis_km"].get<double>(), "km"));
                        hourlyWeather.setProperty(UV, new Property("UV Index", hourData["uv"].get<double>(), ""));
                        hourlyWeather.setProperty(PRECIPITATION, new Property("Precipitation", hourData["precip_mm"].get<double>(), "mm"));
                        forecastDay->addHourlyForecast(j, hourlyWeather);
                    }
                    forecast->addForecastDay(i, forecastDay);
                }
            }

        } catch (const json::parse_error& e) {
            std::cerr << "JSON parse error in APIConverter (getForecast): " << e.what() << std::endl;
            delete forecast;
            forecast = nullptr;
        }
    } else {
        std::cerr << "Error fetching forecast data. Status code: " << (res ? std::to_string(res->status) : "N/A") << std::endl;
    }
    return forecast;
}