// Weather.h
#ifndef WEATHER_H
#define WEATHER_H

#include "Property.h" // Included for Property definition
#include <stdexcept> // For std::runtime_error in assignment operator

// Keep the enum definition
enum PropertyIndex {
    TEMPERATURE = 0,
    FEELS_LIKE = 1,
    WIND_SPEED = 2,
    WIND_DIRECTION = 3,
    HUMIDITY = 4,
    PRESSURE = 5,
    VISIBILITY = 6,
    UV = 7,
    GUST_SPEED = 8,
    PRECIPITATION = 9,
    CLOUD = 10,
    LAST_UPDATED = 11, // Keep epoch time property
    NUM_PROPERTIES = 12 // Number of properties
  };


class Weather {
private:
    Property* properties[NUM_PROPERTIES];

    // Helper to copy properties (used in copy constructor and assignment)
    void copyProperties(const Weather& other);
    // Helper to delete properties (used in destructor and assignment)
    void deleteProperties();


public:
    Weather(); // Default constructor
    ~Weather(); // Destructor

    // --- Rule of Three ---
    Weather(const Weather& other); // Copy constructor (declaration)
    Weather& operator=(const Weather& other); // Copy assignment operator (declaration)

    // --- Rule of Five (Optional but good practice if move semantics were needed) ---
    // Weather(Weather&& other) noexcept; // Move constructor
    // Weather& operator=(Weather&& other) noexcept; // Move assignment operator

    void setProperty(PropertyIndex index, Property* property);
    Property* getProperty(PropertyIndex index) const;
};

#endif // WEATHER_H