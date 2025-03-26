// Weather.h
#ifndef WEATHER_H
#define WEATHER_H

#include "Property.h"

enum PropertyIndex {
    TEMPERATURE = 0,
    HUMIDITY = 1,
    WIND_SPEED = 2,
    // Add more properties as needed
    NUM_PROPERTIES = 3 // UPDATE to Latest Index
};

class Weather {
private:
    Property* properties[NUM_PROPERTIES];
public:
    Weather();
    ~Weather();
    void setProperty(PropertyIndex index, Property* property);
    Property* getProperty(PropertyIndex index) const;
};

#endif // WEATHER_H