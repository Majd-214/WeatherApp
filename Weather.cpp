// Weather.cpp
#include "Weather.h"
#include "Property.h" // Definition of Property needed for new/delete
#include <iostream>   // For cerr (error output in setProperty)
#include <iomanip>    // For stream manipulators (setw, setprecision, left, fixed)
#include <vector>     // For grouping properties during display
#include <ctime>      // For time formatting (strftime, localtime_s/r)
#include <utility>    // For std::move (needed if implementing Rule of Five)
#include <ostream>    // Included via header, good practice

// --- Private Helper Functions ---

// Performs a deep copy of properties from 'other' to 'this' object.
// Assumes 'this->properties' has been cleared or is uninitialized.
void Weather::copyProperties(const Weather& other) {
    for (int i = 0; i < NUM_PROPERTIES; ++i) {
        if (other.properties[i] != nullptr) {
            // Create a new copy of the Property object.
            this->properties[i] = new Property(*(other.properties[i]));
        } else {
            this->properties[i] = nullptr; // Ensure null pointers are copied as null.
        }
    }
}

// Deallocates memory for all non-null Property pointers in the array.
// Sets pointers to nullptr after deletion to prevent dangling pointers.
void Weather::deleteProperties() {
    for (int i = 0; i < NUM_PROPERTIES; ++i) {
        delete properties[i]; // delete handles nullptr safely.
        properties[i] = nullptr; // Set to null after deleting.
    }
}

// --- Constructor / Destructor ---

// Default Constructor: Initialize all pointers in the array to nullptr.
Weather::Weather() {
    for (int i = 0; i < NUM_PROPERTIES; ++i) {
        properties[i] = nullptr;
    }
}

// Destructor: Ensure all dynamically allocated properties are deleted.
Weather::~Weather() {
    deleteProperties();
}

// --- Rule of Three Implementation ---

// Copy Constructor: Perform a deep copy using the helper function.
Weather::Weather(const Weather& other) {
    // Initialize pointers to null before copying (good practice, though copyProperties handles it).
    for (int i = 0; i < NUM_PROPERTIES; ++i) { properties[i] = nullptr; }
    copyProperties(other);
}

// Copy Assignment Operator: Implement using copy-and-swap idiom or clear-and-copy.
Weather& Weather::operator=(const Weather& other) {
    // 1. Self-assignment check.
    if (this != &other) {
        // 2. Clear existing resources in 'this' object.
        deleteProperties();
        // 3. Copy resources from 'other' object.
        copyProperties(other);
    }
    // 4. Return a reference to 'this' object.
    return *this;
}

// --- Rule of Five Implementation (Optional - Uncomment and implement if needed) ---
/*
// Move Constructor: Transfer ownership of resources.
Weather::Weather(Weather&& other) noexcept {
    // Default-initialize 'this' properties (or set explicitly to nullptr).
    for (int i = 0; i < NUM_PROPERTIES; ++i) { this->properties[i] = nullptr; }

    // Transfer ownership by swapping pointers.
    for (int i = 0; i < NUM_PROPERTIES; ++i) {
        std::swap(this->properties[i], other.properties[i]);
    }
    // 'other' is now in a valid, destructible state (all pointers are nullptr).
}

// Move Assignment Operator: Transfer ownership using copy-and-swap or clear-and-move.
Weather& Weather::operator=(Weather&& other) noexcept {
    // 1. Self-assignment check (though less critical for move).
    if (this != &other) {
        // 2. Clear existing resources in 'this'.
        deleteProperties();

        // 3. Transfer ownership from 'other' to 'this'.
        for (int i = 0; i < NUM_PROPERTIES; ++i) {
            this->properties[i] = other.properties[i];
            other.properties[i] = nullptr; // Leave 'other' in a valid state.
        }
        // Alternative using swap:
        // for (int i = 0; i < NUM_PROPERTIES; ++i) {
        //     std::swap(this->properties[i], other.properties[i]);
        // }
    }
    // 4. Return reference to 'this'.
    return *this;
}
*/

// --- Property Management ---

// Assigns a Property pointer to the specified index, managing memory.
void Weather::setProperty(PropertyIndex index, Property* property) {
    // Check if the index is valid.
    if (index >= 0 && index < NUM_PROPERTIES) {
        // Delete the existing property at this index, if any.
        delete properties[index];
        // Assign the new property pointer. Ownership is transferred to Weather object.
        properties[index] = property;
    } else {
        // Index is out of bounds. Log an error.
        std::cerr << "Error: Invalid property index (" << index << ") in setProperty." << std::endl;
        // IMPORTANT: Delete the passed-in pointer to prevent a memory leak,
        // as the Weather object cannot take ownership due to invalid index.
        delete property;
    }
}

// Retrieves a raw pointer to the Property at the given index.
Property* Weather::getProperty(PropertyIndex index) const {
    // Check for valid index.
    if (index >= 0 && index < NUM_PROPERTIES) {
        // Return the pointer (which might be nullptr if not set).
        return properties[index];
    }
    // Index is out of bounds, return nullptr.
    return nullptr;
}

// --- Display Helper ---

// Displays the weather data in a structured format to the output stream.
void Weather::displayData(std::ostream& os) const {
    // Define groups of related properties for logical output structuring.
    const std::vector<PropertyIndex> tempGroup = {TEMPERATURE, FEELS_LIKE};
    const std::vector<PropertyIndex> windGroup = {WIND_SPEED, GUST_SPEED, WIND_DIRECTION};
    const std::vector<PropertyIndex> atmosGroup = {PRECIPITATION, HUMIDITY, CLOUD}; // Order changed slightly
    const std::vector<PropertyIndex> otherGroup = {PRESSURE, VISIBILITY, UV};
    const std::vector<PropertyIndex> timeGroup = {LAST_UPDATED};

    bool dataDisplayed = false; // Flag to track if any data was actually printed.

    // Lambda function to print a group of properties.
    auto printGroup = [&](const std::vector<PropertyIndex>& group) {
        for (PropertyIndex index : group) {
            const Property* prop = getProperty(index); // Get the property pointer.

            if (prop != nullptr) { // Check if the property exists.

                // Special handling for LAST_UPDATED: format epoch time.
                if (index == LAST_UPDATED) {
                    // Safely convert double (epoch seconds) to time_t.
                    time_t epochTime = static_cast<time_t>(prop->getValue());
                    std::tm timeinfo = {}; // Zero-initialize tm struct.

                    // Use platform-specific safe time conversion functions.
                    #ifdef _WIN32
                         localtime_s(&timeinfo, &epochTime);
                    #else // POSIX
                         localtime_r(&epochTime, &timeinfo);
                    #endif

                    // Buffer for formatted time string.
                    char time_buf[100];
                    // Format time as "YYYY-MM-DD HH:MM:SS Timezone".
                    if (std::strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S %Z", &timeinfo)) {
                        os << "  " << std::left << std::setw(15) << prop->getName() << ": " << time_buf << std::endl;
                    } else {
                        // Handle potential strftime error (buffer too small, etc.)
                         os << "  " << std::left << std::setw(15) << prop->getName() << ": " << "(time formatting error)" << std::endl;
                    }
                }
                // Handling for other properties.
                else {
                    // Basic formatting for numerical properties.
                     os << "  " << std::left << std::setw(15) << prop->getName() << ": "
                       << std::fixed << std::setprecision(1) // Show one decimal place.
                       << prop->getValue();
                    // Append unit if it's not empty.
                    if (!prop->getUnit().empty()) { os << " " << prop->getUnit(); }
                    os << std::endl;
                }
                dataDisplayed = true; // Mark that we have displayed something.
            }
        }
    };

    // Print each group of properties using the lambda.
    printGroup(tempGroup);
    printGroup(windGroup);
    printGroup(atmosGroup);
    printGroup(otherGroup);
    printGroup(timeGroup);

    // If no properties were found or displayed, print a placeholder message.
    if (!dataDisplayed) {
        os << "  (No specific weather data available)" << std::endl;
    }
}