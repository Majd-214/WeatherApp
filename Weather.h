// Weather.h
#ifndef WEATHER_H
#define WEATHER_H

#include "Property.h" // Defines the Property class used in the array
#include <ostream>    // For the displayData method parameter
#include <vector>     // Used in displayData implementation
#include <ctime>      // Used in displayData implementation for time formatting
#include <iomanip>    // Used in displayData implementation for formatting

// Enum defining indices for accessing specific weather properties in the array.
// Provides type safety and readability compared to magic numbers.
enum PropertyIndex {
    TEMPERATURE = 0,
    FEELS_LIKE,
    WIND_SPEED,
    WIND_DIRECTION,
    HUMIDITY,
    PRESSURE,
    VISIBILITY,
    UV,
    GUST_SPEED,
    PRECIPITATION,
    CLOUD,
    LAST_UPDATED,
    NUM_PROPERTIES // Sentinel value indicating the total number of properties
};

// Represents a collection of weather properties at a specific point in time or for a summary period.
// Manages the lifetime of Property objects stored within it.
class Weather {
private:
    // Fixed-size array of pointers to Property objects.
    Property* properties[NUM_PROPERTIES];

    // --- Private Helper Methods for Resource Management (Rule of Three/Five) ---

    // Copies Property objects from another Weather instance (deep copy).
    void copyProperties(const Weather& other);
    // Deletes all dynamically allocated Property objects held by this instance.
    void deleteProperties();

public:
    // Default Constructor: Initializes all property pointers to nullptr.
    Weather();
    // Destructor: Cleans up dynamically allocated Property objects.
    ~Weather();

    // --- Rule of Three (Copy Control) ---

    // Copy Constructor: Performs a deep copy of the other Weather object.
    Weather(const Weather& other);
    // Copy Assignment Operator: Handles self-assignment and performs a deep copy.
    Weather& operator=(const Weather& other);

    // --- Rule of Five (Move Semantics - Optional but recommended for C++11+) ---
    // Move Constructor (Optional): Efficiently transfers ownership of resources.
    // Weather(Weather&& other) noexcept; // Declare if implementing
    // Move Assignment Operator (Optional): Efficiently transfers ownership of resources.
    // Weather& operator=(Weather&& other) noexcept; // Declare if implementing


    // --- Property Management ---

    // Sets a property at the specified index. Takes ownership of the provided 'property' pointer.
    // Deletes any existing property at that index before assignment.
    // Note: If index is invalid, the provided property is deleted to prevent leaks.
    void setProperty(PropertyIndex index, Property* property);

    // Retrieves a non-owning (raw) pointer to the Property object at the specified index.
    // Returns nullptr if the index is invalid or no property is set at that index.
    // The caller MUST NOT delete the returned pointer.
    Property* getProperty(PropertyIndex index) const;

    // --- Display Helper ---

    // Formats and prints the contained weather data to the provided output stream.
    // This is a helper method intended to be called by specific Report classes.
    void displayData(std::ostream& os) const;
};

#endif // WEATHER_H