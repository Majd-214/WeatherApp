//
// Created by Majdp on 27-Mar-2025.
//
#include "APIConverter.h"
#include "nlohmann/json.hpp"
#include <iostream>

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

            if (weatherData.contains("location") && weatherData["location"].contains("name")) {
                std::cout << "Location: " << weatherData["location"]["name"].get<std::string>() << std::endl;
            }
            if (weatherData.contains("current")) {
                if (weatherData["current"].contains("temp_c")) {
                    double tempValue = weatherData["current"]["temp_c"].get<double>();
                    weather->setProperty(TEMPERATURE, new Property("Temperature", tempValue, "Celsius"));
                    std::cout << "Temperature: " << weather->getProperty(TEMPERATURE)->getValue() << " " << weather->getProperty(TEMPERATURE)->getUnit() << std::endl;
                }
                if (weatherData["current"].contains("humidity")) {
                    double humidityValue = weatherData["current"]["humidity"].get<double>();
                    weather->setProperty(HUMIDITY, new Property("Humidity", humidityValue, "%"));
                    std::cout << "Humidity: " << weather->getProperty(HUMIDITY)->getValue() << weather->getProperty(HUMIDITY)->getUnit() << std::endl;
                }
                if (weatherData["current"].contains("wind_kph")) {
                    double windSpeedValue = weatherData["current"]["wind_kph"].get<double>();
                    weather->setProperty(WIND_SPEED, new Property("Wind Speed", windSpeedValue, "km/h"));
                    std::cout << "Wind Speed: " << weather->getProperty(WIND_SPEED)->getValue() << " " << weather->getProperty(WIND_SPEED)->getUnit() << std::endl;
                }
                // Add more properties as needed from the API response
            }

        } catch (const json::parse_error& e) {
            std::cerr << "JSON parse error in APIConverter: " << e.what() << std::endl;
        }
    } else {
        std::cerr << "Error fetching weather data. Status code: " << (res ? std::to_string(res->status) : "N/A") << std::endl;
    }
}