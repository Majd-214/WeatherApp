// Weather.cpp
#include "Weather.h"
#include "Property.h" // Include needed for 'new Property'
#include <iostream>   // For cerr
#include <utility>    // For std::move (if implementing move semantics)

// --- Helper Functions ---
void Weather::copyProperties(const Weather& other) {
     for (int i = 0; i < NUM_PROPERTIES; ++i) {
        if (other.properties[i] != nullptr) {
            // Create a *new* Property object, copying data from the other's property
            properties[i] = new Property(other.properties[i]->getName(),
                                         other.properties[i]->getValue(),
                                         other.properties[i]->getUnit());
        } else {
            properties[i] = nullptr;
        }
    }
}

void Weather::deleteProperties() {
     for (int i = 0; i < NUM_PROPERTIES; ++i) {
        delete properties[i]; // delete is safe on nullptr
        properties[i] = nullptr; // Set to nullptr after deletion
    }
}


// --- Constructor / Destructor ---
Weather::Weather() {
    // Initialize all pointers to nullptr
    for (int i = 0; i < NUM_PROPERTIES; ++i) {
        properties[i] = nullptr;
    }
}

Weather::~Weather() {
    deleteProperties(); // Use helper
}

// --- Rule of Three Implementation ---

// Copy Constructor
Weather::Weather(const Weather& other) {
    copyProperties(other); // Use helper
}

// Copy Assignment Operator
Weather& Weather::operator=(const Weather& other) {
    // 1. Self-assignment check
    if (this == &other) {
        return *this;
    }

    // 2. Delete existing resources
    deleteProperties();

    // 3. Copy resources from 'other'
    copyProperties(other);

    // 4. Return reference to self
    return *this;
}


// --- Property Management ---
void Weather::setProperty(PropertyIndex index, Property* property) {
    if (index >= 0 && index < NUM_PROPERTIES) {
        // Delete the old property at this index before assigning the new one
        if (properties[index] != nullptr) {
            delete properties[index];
        }
        properties[index] = property; // Assign the new property pointer
    } else {
        std::cerr << "Error: Invalid property index used in setProperty: " << index << std::endl;
        // Should we delete the passed 'property' here to avoid memory leak?
        // Depends on ownership rules. If setProperty takes ownership, yes.
        delete property; // Assuming setProperty takes ownership if index is invalid
    }
}

Property* Weather::getProperty(PropertyIndex index) const {
    if (index >= 0 && index < NUM_PROPERTIES) {
        return properties[index];
    } else {
        // Avoid cerr in a const method if possible, or make it non-modifying
        // Consider logging or returning a specific error indicator if needed
        // Returning nullptr is often sufficient indication of an error here.
        return nullptr;
    }
}