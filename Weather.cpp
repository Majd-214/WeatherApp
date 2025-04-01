// Weather.cpp
#include "Weather.h"
#include "Property.h" // Definition of Property needed for new/delete
#include <iostream>   // For cerr (error output in setProperty)
#include <iomanip>    // For stream manipulators (setw, setprecision, left, fixed)
#include <vector>     // For grouping properties during display
#include <ctime>      // For time formatting (strftime, localtime_s/r)
#include <utility>    // For std::move (needed if implementing Rule of Five)
#include <ostream>    // Included via header, good practice
#include <string>     // For std::string
#include <cmath>      // For std::fmod

// --- Internal Helper Functions (Anonymous Namespace) ---
namespace {
    // Converts wind direction degrees to a cardinal direction string (e.g., N, NE, E).
    std::string degreesToCardinal(double degrees) {
        degrees = std::fmod(degrees, 360.0);
        if (degrees < 0) degrees += 360.0;
        const std::string directions[] = {"N", "NE", "E", "SE", "S", "SW", "W", "NW", "N"};
        int index = static_cast<int>((degrees + 22.5) / 45.0);
        return directions[index % 8];
    }
} // end anonymous namespace


// --- Private Helper Functions ---

void Weather::copyProperties(const Weather& other) {
    for (int i = 0; i < NUM_PROPERTIES; ++i) {
        if (other.properties[i] != nullptr) {
            this->properties[i] = new Property(*(other.properties[i]));
        } else {
            this->properties[i] = nullptr;
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
    for (int i = 0; i < NUM_PROPERTIES; ++i) { properties[i] = nullptr; }
    copyProperties(other);
}

Weather& Weather::operator=(const Weather& other) {
    if (this != &other) {
        deleteProperties();
        copyProperties(other);
    }
    return *this;
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

Property* Weather::getProperty(PropertyIndex index) const {
    if (index >= 0 && index < NUM_PROPERTIES) {
        return properties[index];
    }
    return nullptr;
}


// --- Display Helper ---

// Displays the weather data in a structured list format to the output stream.
// Wind direction includes both cardinal and degrees. Subtitles removed.
void Weather::displayData(std::ostream& os) const {
    // Define groups of related properties for logical output structuring.
    // Simpler grouping compared to previous subtitle version.
    const std::vector<PropertyIndex> displayOrder = {
        TEMPERATURE, FEELS_LIKE,
        WIND_SPEED, GUST_SPEED, WIND_DIRECTION, // Wind group
        PRECIPITATION, HUMIDITY, CLOUD, PRESSURE, // Conditions group
        VISIBILITY, UV,                          // Other group
        LAST_UPDATED                             // Time group
    };

    bool dataDisplayed = false; // Flag to track if any data was actually printed.

    // Iterate through the defined order and print properties if they exist.
    for (PropertyIndex index : displayOrder) {
        const Property* prop = getProperty(index);
        if (prop != nullptr) {
            dataDisplayed = true; // Mark that we found something to display

            // Standard formatting for most properties
            os << "  " << std::left << std::setw(15) << prop->getName() << ": ";

            // Special handling for specific properties
            if (index == WIND_DIRECTION) {
                // Display both cardinal direction and degrees for wind direction.
                os << degreesToCardinal(prop->getValue())
                   << " (" << std::fixed << std::setprecision(1) << prop->getValue() << "\370)"; // Using \370 for degree symbol

            } else if (index == LAST_UPDATED) {
                // Format epoch time for Last Updated.
                time_t epochTime = static_cast<time_t>(prop->getValue());
                std::tm timeinfo = {};
                #ifdef _WIN32
                     localtime_s(&timeinfo, &epochTime);
                #else // POSIX
                     localtime_r(&epochTime, &timeinfo);
                #endif
                char time_buf[100];
                if (std::strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S %Z", &timeinfo)) {
                    os << time_buf;
                } else {
                    os << "(time formatting error)";
                }
            } else {
                // Default formatting for numerical values.
                os << std::fixed << std::setprecision(1) << prop->getValue();
                if (!prop->getUnit().empty()) {
                    os << " " << prop->getUnit();
                }
            }
            os << std::endl; // Newline after each property line
        }
    }

    // If no properties were found or displayed at all, print a placeholder message.
    if (!dataDisplayed) {
        os << "  (No specific weather data available)" << std::endl;
    }
}