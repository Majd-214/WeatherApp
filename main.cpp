#include <iostream>
#include "HTTP/cpp-httplib/httplib.h"
#include "nlohmann/json.hpp"
#include "Property.h"
#include "Weather.h"

using json = nlohmann::json;
using namespace std;

int main() {
    string apiKey = "e2cf72779019414bab7195316251003"; // Replace with your actual API key
    string location = "Hamilton, Ontario, Canada";

    httplib::Client cli("http://api.weatherapi.com");
    auto res = cli.Get(("/v1/current.json?key=" + apiKey + "&q=" + location).c_str());

    if (res && res->status == 200) {
        try {
            json weatherData = json::parse(res->body.c_str(), res->body.c_str() + res->body.length());

            Weather currentWeather;

            if (weatherData.contains("location") && weatherData["location"].contains("name")) {
                cout << "Location: " << weatherData["location"]["name"].get<string>() << endl;
            }
            if (weatherData.contains("current")) {
                if (weatherData["current"].contains("temp_c")) {
                    double tempValue = weatherData["current"]["temp_c"].get<double>();
                    currentWeather.setProperty(TEMPERATURE, new Property("Temperature", tempValue, "Celsius"));
                    cout << "Temperature: " << currentWeather.getProperty(TEMPERATURE)->getValue() << " " << currentWeather.getProperty(TEMPERATURE)->getUnit() << endl;
                }
                if (weatherData["current"].contains("humidity")) {
                    double humidityValue = weatherData["current"]["humidity"].get<double>();
                    currentWeather.setProperty(HUMIDITY, new Property("Humidity", humidityValue, "%"));
                    cout << "Humidity: " << currentWeather.getProperty(HUMIDITY)->getValue() << currentWeather.getProperty(HUMIDITY)->getUnit() << endl;
                }
            }

        } catch (const json::parse_error& e) {
            cerr << "JSON parse error: " << e.what() << endl;
        }
    } else {
        cerr << "Error fetching weather data" << endl;
    }

    return 0;
}