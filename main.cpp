#include <Property.h>
#include <Weather.h>
#include <iostream>

using namespace std;

int main() {
  Weather weather;

  const char *PropertyNames[NUM_PROPERTIES] = {
      "Temperature",    "Feels Like", "High Temp",  "Low Temp",
      "Air Quality",    "UV",         "Wind Speed", "Gust",
      "Wind Direction", "Sunrise",    "Sunset",     "Precipitation",
      "Visibility",     "Humidity",   "Pressure"};

  const char *PropertyUnits[NUM_PROPERTIES] = {
      "°C", "°C", "°C", "°C", "AQI", "",  "km/h", "km/h",
      "°",  "AM", "PM", "mm", "km",  "%", "hPa"};

  double InputValues;
  for (int i = 0; i < NUM_PROPERTIES; i++) {
    cout << "Enter value for " << PropertyNames[i] << " (" << PropertyUnits[i]
         << "): ";
    cin >> InputValues;

    Property *p = new Property(PropertyNames[i], InputValues, PropertyUnits[i]);
    weather.setProperty(static_cast<PropertyIndex>(i), p); // chat
  }
  // chat
  cout << "\nWeather Data for the day:\n";
  // Loop to output the weather data.
  for (int i = 0; i < NUM_PROPERTIES; i++) {
    Property *p = weather.getProperty(static_cast<PropertyIndex>(i));
    if (p) {
      cout << *p;
    } else {
      cout << PropertyNames[i] << ": N/A" << endl;
    }
  }
}