// Preferences.cpp
#include "Preferences.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <stdexcept> // For exceptions if desired over bool returns

namespace { // Use anonymous namespace for internal linkage helpers
    // Helper function to trim leading/trailing whitespace
    std::string trimInternal(const std::string& str) {
        size_t first = str.find_first_not_of(" \t\n\r\f\v");
        if (std::string::npos == first) {
            return str;
        }
        size_t last = str.find_last_not_of(" \t\n\r\f\v");
        return str.substr(first, (last - first + 1));
    }

    // Helper function to convert string to lower case
    std::string toLowerInternal(const std::string& str) {
        std::string lowerStr = str;
        std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(),
                       [](unsigned char c){ return std::tolower(c); });
        return lowerStr;
    }
} // anonymous namespace


// --- Private Helpers ---
std::string Preferences::trim(const std::string& str) { return trimInternal(str); }
std::string Preferences::toLower(const std::string& str) { return toLowerInternal(str); }

void Preferences::loadDefaults() {
    apiKey = ""; // Default empty - requires user input or file
    location = "London"; // Default location
    units = "Metric"; // Default units
    datamode = "advanced"; // Default mode
    forecastDays = 3; // Default days
}

// --- Constructor ---
Preferences::Preferences(const std::string& filename) : settingsFilename(filename) {
    loadDefaults(); // Start with defaults
    loadSettings(); // Try to load from file, potentially overwriting defaults
}

// --- Getters ---
const std::string& Preferences::getApiKey() const { return apiKey; }
const std::string& Preferences::getLocation() const { return location; }
const std::string& Preferences::getUnits() const { return units; }
const std::string& Preferences::getDataMode() const { return datamode; }
int Preferences::getForecastDays() const { return forecastDays; }

// --- Setters ---
void Preferences::setApiKey(const std::string& key) { apiKey = key; } // No validation assumed needed here
void Preferences::setLocation(const std::string& loc) { location = trim(loc); } // Trim location input

bool Preferences::setUnits(const std::string& unit) {
    std::string trimmedUnit = trim(unit);
    if (trimmedUnit == "Metric" || trimmedUnit == "Imperial") {
        units = trimmedUnit;
        return true;
    }
    std::cerr << "Warning: Invalid unit '" << unit << "' provided. Units remain '" << units << "'." << std::endl;
    return false;
}

void Preferences::setDataMode(const std::string& mode) { datamode = trim(mode); }

bool Preferences::setForecastDays(int days) {
    if (days >= 1 && days <= 14) { // WeatherAPI typical limit
        forecastDays = days;
        return true;
    }
    std::cerr << "Warning: Invalid forecast days value '" << days << "'. Forecast days remain '" << forecastDays << "'." << std::endl;
    return false;
}

// --- File Operations ---
bool Preferences::loadSettings() {
    std::ifstream infile(settingsFilename);
    if (!infile.is_open()) {
        std::cerr << "Info: Could not open settings file '" << settingsFilename << "'. Using default/current settings." << std::endl;
        // Optionally create a default file on first run if it doesn't exist?
        // saveSettings();
        return false;
    }

    std::string line;
    bool loadedSomething = false;
    while (getline(infile, line)) {
        std::istringstream linestream(line);
        std::string key;
        if (getline(linestream, key, ':')) {
            std::string value;
            getline(linestream, value); // Read rest of line
            std::string lowerKey = toLower(trim(key));
            value = trim(value);

            if (value.empty()) continue; // Skip lines without values

            if (lowerKey == "apikey") { setApiKey(value); loadedSomething = true; }
            else if (lowerKey == "location") { setLocation(value); loadedSomething = true; }
            else if (lowerKey == "units") { if(setUnits(value)) loadedSomething = true; }
            else if (lowerKey == "datamode") { setDataMode(value); loadedSomething = true; }
            else if (lowerKey == "forecastdays") {
                try {
                    int days = std::stoi(value);
                    if(setForecastDays(days)) loadedSomething = true;
                } catch (const std::invalid_argument&) {
                    std::cerr << "Warning: Invalid number format for forecastdays '" << value << "' in settings file." << std::endl;
                } catch (const std::out_of_range&) {
                     std::cerr << "Warning: Forecastdays value '" << value << "' out of range in settings file." << std::endl;
                }
            }
        }
    }
    infile.close();
    if (loadedSomething) {
        std::cout << "Settings loaded successfully from '" << settingsFilename << "'" << std::endl;
    }
    return true; // Return true even if file was just empty or only had warnings
}

bool Preferences::saveSettings() const {
    std::ofstream outfile(settingsFilename);
    if (!outfile.is_open()) {
        std::cerr << "Error: Could not open settings file '" << settingsFilename << "' for saving." << std::endl;
        return false;
    }

    outfile << "apikey:" << apiKey << std::endl;
    outfile << "location:" << location << std::endl;
    outfile << "units:" << units << std::endl;
    outfile << "datamode:" << datamode << std::endl;
    outfile << "forecastdays:" << forecastDays << std::endl;

    outfile.close();

    if (!outfile) { // Check stream state after closing
         std::cerr << "Error: Failed to write all data to settings file '" << settingsFilename << "'." << std::endl;
         return false;
    }
    std::cout << "Settings saved successfully to '" << settingsFilename << "'" << std::endl;
    return true;
}