// Weather.h
#ifndef WEATHER_H
#define WEATHER_H

#include "Property.h" // For Property definition
#include <vector>     // Include vector as it might be used internally later
#include <ostream>    // For display helper parameter type
#include <map>        // Could use map instead of array for properties
#include <memory>     // For potential future use of smart pointers

// PropertyIndex enum remains useful for direct access if needed
// Could be replaced if switching to std::map<string, Property*>
enum PropertyIndex {
    TEMPERATURE = 0, FEELS_LIKE, WIND_SPEED, WIND_DIRECTION, HUMIDITY,
    PRESSURE, VISIBILITY, UV, GUST_SPEED, PRECIPITATION, CLOUD, LAST_UPDATED,
    NUM_PROPERTIES // Size indicator
  };

// Represents weather conditions at a specific point in time
class Weather {
    private:
    // Using an array still, ensure NUM_PROPERTIES is accurate
    Property* properties[NUM_PROPERTIES];

    // Private helpers for copy control (Rule of Three)
    void copyProperties(const Weather& other);
    void deleteProperties();

    public:
    Weather();
    ~Weather();

    // Rule of Three (Deep Copy Implementation)
    Weather(const Weather& other);
    Weather& operator=(const Weather& other);

    // --- Property Management ---
    // Sets property, taking ownership of the pointer if index is valid.
    // Deletes existing property at that index first.
    void setProperty(PropertyIndex index, Property* property);
    // Returns raw pointer, caller does NOT own it. Returns nullptr if index invalid.
    Property* getProperty(PropertyIndex index) const;

    // --- Helper for Display ---
    // Non-virtual display helper to be called by report classes
    void displayData(std::ostream& os) const;
};

#endif // WEATHER_H