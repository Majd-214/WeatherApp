// Weather.cpp
#include "Weather.h"
#include "Property.h"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <vector>
#include <utility> // For std::move if needed later

// --- Helper Functions ---
void Weather::copyProperties(const Weather& other) {
    for (int i = 0; i < NUM_PROPERTIES; ++i) {
        if (other.properties[i] != nullptr) {
            properties[i] = new Property(*(other.properties[i]));
        } else {
            properties[i] = nullptr;
        }
    }
}

void Weather::deleteProperties() {
    for (int i = 0; i < NUM_PROPERTIES; ++i) {
        delete properties[i];
        properties[i] = nullptr;
    }
}

// --- Constructor / Destructor ---
Weather::Weather() {
    for (int i = 0; i < NUM_PROPERTIES; ++i) {
        properties[i] = nullptr;
    }
}

Weather::~Weather() {
    deleteProperties();
}

// --- Rule of Three Implementation ---
Weather::Weather(const Weather& other) {
    copyProperties(other);
}

// Copy Assignment Operator - FIXED
Weather& Weather::operator=(const Weather& other) {
    if (this != &other) { // Self-assignment check
        deleteProperties(); // Clear current state
        copyProperties(other); // Copy from other
    }
    return *this; // <<< Added missing return statement
}

// --- Property Management ---
void Weather::setProperty(PropertyIndex index, Property* property) {
    if (index >= 0 && index < NUM_PROPERTIES) {
        delete properties[index];
        properties[index] = property;
    } else {
        std::cerr << "Error: Invalid property index (" << index << ") in setProperty." << std::endl;
        delete property;
    }
}

// getProperty - FIXED
Property* Weather::getProperty(PropertyIndex index) const {
    if (index >= 0 && index < NUM_PROPERTIES) {
        return properties[index]; // <<< Added missing return statement
    }
    return nullptr; // <<< Added return for invalid index case
}


// --- Display Helper ---
void Weather::displayData(std::ostream& os) const {
    std::vector<PropertyIndex> tempGroup = {TEMPERATURE, FEELS_LIKE};
    std::vector<PropertyIndex> windGroup = {WIND_SPEED, GUST_SPEED, WIND_DIRECTION};
    std::vector<PropertyIndex> atmosGroup = {PRECIPITATION, CLOUD, HUMIDITY};
    std::vector<PropertyIndex> otherGroup = {PRESSURE, VISIBILITY, UV};
    std::vector<PropertyIndex> timeGroup = {LAST_UPDATED};

    bool dataDisplayed = false;

    auto printGroup = [&](const std::vector<PropertyIndex>& group) {
        // bool groupHasData = false; // Not strictly needed now
        for (PropertyIndex index : group) {
            const Property* prop = getProperty(index); // Use the fixed getProperty
            if (prop != nullptr) {
                 if (index == PRECIPITATION && prop->getValue() == 0.0 && group.size() > 1) continue; // Hide 0 precip if other things in group

                 if (index == LAST_UPDATED) {
                     // ... (time formatting code) ...
                     time_t epochTime = static_cast<time_t>(prop->getValue());
                     std::tm timeinfo = {};
                     #ifdef _WIN32
                          localtime_s(&timeinfo, &epochTime);
                     #else
                          localtime_r(&epochTime, &timeinfo);
                     #endif
                     char time_buf[100];
                     if (std::strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S %Z", &timeinfo)) {
                         os << "  " << std::left << std::setw(15) << prop->getName() << ": " << time_buf << std::endl;
                     } else { /* ... error handling ... */ }
                 } else {
                     // ... (standard property formatting code) ...
                      os << "  " << std::left << std::setw(15) << prop->getName() << ": "
                        << std::fixed << std::setprecision(1)
                        << prop->getValue();
                     if (!prop->getUnit().empty()) { os << " " << prop->getUnit(); }
                     os << std::endl;
                 }
                 // groupHasData = true;
                 dataDisplayed = true;
            }
        }
    };

    printGroup(tempGroup);
    printGroup(windGroup);
    printGroup(atmosGroup);
    printGroup(otherGroup);
    printGroup(timeGroup);

    if (!dataDisplayed) {
        os << "  (No specific weather data available)" << std::endl;
    }
}