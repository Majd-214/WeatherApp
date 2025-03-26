// Weather.cpp
#include "Weather.h"
#include "Property.h"
#include <iostream>

using namespace std;

Weather::Weather() {
    for (int i = 0; i < NUM_PROPERTIES; ++i) {
        properties[i] = nullptr;
    }
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