#include "Preferences.h"

#include <Property.h>
#include <Weather.h>
#include <iostream>
#include <string>
#include "APIConverter.h"
#include "Weather.h"

using namespace std;

int main() {

  Preferences preferences;

  if (!preferences.fileloading(("settings.txt"))) {
    cout << "Settings fileloading failed. Using default preferences." << endl;
  }

  cout << "Preferences Currently: " << endl;
  cout << "Location: " << preferences.location << endl;
  cout << "Units: " << preferences.units << endl;
  cout << "Data mode: " << preferences.datamode << endl;

  int NumPropertiesUsage;
  const char **PropertyNames;
  const char **PropertyUnits;

  const char *PropertyNamesAdv[NUM_PROPERTIES] = {
      "Temperature",    "Feels Like", "High Temp",  "Low Temp",
      "Air Quality",    "UV",         "Wind Speed", "Gust",
      "Wind Direction", "Sunrise",    "Sunset",     "Precipitation",
      "Visibility",     "Humidity",   "Pressure"};

  const char *PropertyUnitsAdv[NUM_PROPERTIES] = {
      "\370C", "\370C", "\370C", "\370C", "AQI", "",  "km/h", "km/h",
      "\370",  "AM",    "PM",    "mm",    "km",  "%", "hPa"};

  const char *PropertyNamesEss[4] = {
      "Temperature",
      "Feels Like",
      "High Temp",
      "Low Temp",
  };

  const char *PropertyUnitsEss[4] = {"\370C", "\370C", "\370C", "\370C"};

  if (preferences.datamode == "Advanced" ||
      preferences.datamode == "advanced") {
    NumPropertiesUsage = NUM_PROPERTIES;
    PropertyNames = PropertyNamesAdv;
    PropertyUnits = PropertyUnitsEss;
  }

  Weather weather;

  PropertyIndex Variables[NUM_PROPERTIES] = {
      TEMPERATURE, FEELS_LIKE,    HIGH_TEMP,  LOW_TEMP,       AIR_QUALITY,
      UV,          WIND_SPEED,    GUST,       WIND_DIRECTION, SUNRISE,
      SUNSET,      PRECIPITATION, VISIBILITY, HUMIDITY,       PRESSURE};

  double InputValues;
  for (int i = 0; i < NUM_PROPERTIES; i++) {
    cout << "Enter value for " << PropertyNames[i] << " (" << PropertyUnits[i]
         << "): ";
    cin >> InputValues;

    Property *property =
        new Property(PropertyNames[i], InputValues, PropertyUnits[i]);
    weather.setProperty(Variables[i], property);
  }

  cout << "Weather Data for the day: \n";

  for (int i = 0; i < NUM_PROPERTIES; i++) {
    Property *property = weather.getProperty(Variables[i]);

    if (property) {
      cout << *property;
    }

    else {
      cout << PropertyNames[i] << " not found\n" << endl;
    }
  }
  
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