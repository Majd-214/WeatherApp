// Weather.cpp
#include "Weather.h"
#include "Property.h"
#include <iostream>

using namespace std;

Weather::Weather()  : hour(0), temperature(0.0), condition(" "){
    for (int i = 0; i < NUM_PROPERTIES; ++i) {
        properties[i] = nullptr;
    }
}
Weather::Weather(int h, double temp, const string &cond) : hour(h), temperature(temp), condition(cond) {
}

Weather::~Weather() {
    for (int i = 0; i < NUM_PROPERTIES; ++i) {
        if (properties[i] != nullptr) {
            delete properties[i];
        }
    }
}

void Weather::setProperty(PropertyIndex index, Property* property) {
    if (index >= 0 && index < NUM_PROPERTIES) {
        if (properties[index] != nullptr) {
            delete properties[index];
        }
        properties[index] = property;
    } else {
        cerr << "Error: Invalid property index: " << index << endl;
    }
}

Property* Weather::getProperty(PropertyIndex index) const {
    if (index >= 0 && index < NUM_PROPERTIES) {
        return properties[index];
    } else {
        cerr << "Error: Invalid property index: " << index << endl;
        return nullptr;
    }
}

int Weather::getHour() const {
    return hour;
}

double Weather::getTemperature() const {
    return temperature;
}

std::string Weather::getCondition() const {
    return condition;
}

void Weather::setHour(int h) {
    hour = h;
}

void Weather::setTemperature(double temp) {
    temperature = temp;
}

void Weather::setCondition(const string &cond) {
    condition = cond;
}

void Weather::display() const {
    cout << "Hour: " << hour
         << ", Temp: " << temperature << "°C"
         << ", Condition: " << condition << endl;
}