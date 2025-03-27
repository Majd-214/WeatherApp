#include <iostream>
#include <string>
#include "APIConverter.h"
#include "Weather.h"

using namespace std;

int main() {
    // Replace with your actual API key and desired location
    APIConverter apiConverter("e2cf72779019414bab7195316251003", "Hamilton, Ontario, Canada");

    // Create a Weather object
    Weather currentWeather;

    // Update the weather data
    apiConverter.updateWeather(&currentWeather);

    // You can now work with the currentWeather object
    // For example, access properties like this:
    if (currentWeather.getProperty(TEMPERATURE)) {
        cout << "Temperature: " << currentWeather.getProperty(TEMPERATURE)->getValue() << endl;
    }
    if (currentWeather.getProperty(HUMIDITY)) {
        cout << "Humidity: " << currentWeather.getProperty(HUMIDITY)->getValue() << endl;
    }
    if (currentWeather.getProperty(WIND_SPEED)) {
        cout << "Wind Speed: " << currentWeather.getProperty(WIND_SPEED)->getValue() << endl;
    }

    return 0;
}