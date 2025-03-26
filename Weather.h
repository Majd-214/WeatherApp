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
protected:
    double temperature;
    int hour;
    string condition;

    Property* properties[NUM_PROPERTIES];
public:

    ~Weather();
    void setProperty(PropertyIndex index, Property* property);
    Property* getProperty(PropertyIndex index) const;

    // Constructors
    Weather();
    Weather(int h, double temp, const std::string &cond);

    // Getters
    int getHour() const;
    double getTemperature() const;
    string getCondition() const;

    // Setters
    void setHour(int h);
    void setTemperature(double temp);
    void setCondition(const std::string &cond);

    virtual void display() const;
};




#endif // WEATHER_H