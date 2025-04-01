// Preferences.cpp
#include "Preferences.h"
#include <fstream>   // For file stream operations (ifstream, ofstream)
#include <iostream>  // For error/info messages (cerr, cout)
#include <sstream>   // For parsing lines from the settings file (istringstream)
#include <string>    // For string operations
#include <algorithm> // For std::transform (used in toLower)
#include <cctype>    // For std::tolower
#include <stdexcept> // For exception handling during string conversion (stoi)

// --- Internal Helper Functions (Anonymous Namespace) ---
namespace {
    // Removes leading and trailing whitespace (space, tab, newline, etc.) from a string.
    std::string trimInternal(const std::string& str) {
        size_t first = str.find_first_not_of(" \t\n\r\f\v");
        if (std::string::npos == first) { // String is all whitespace or empty
            return "";
        }
        size_t last = str.find_last_not_of(" \t\n\r\f\v");
        return str.substr(first, (last - first + 1));
    }

    // Converts a string to its lowercase equivalent.
    std::string toLowerInternal(const std::string& str) {
        std::string lowerStr = str;
        std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(),
                       [](unsigned char c){ return std::tolower(c); });
        return lowerStr;
    }
} // end anonymous namespace

// --- Private Class Helpers (Calling internal helpers) ---

// Publicly accessible trim via private member (if needed elsewhere, but internal is sufficient here).
std::string Preferences::trim(const std::string& str) { return trimInternal(str); }
// Publicly accessible toLower via private member.
std::string Preferences::toLower(const std::string& str) { return toLowerInternal(str); }

// Sets the default values for all preferences.
void Preferences::loadDefaults() {
    apiKey = "";          // API key must be provided by user or file.
    location = "London";  // Default location if not set.
    units = "Metric";     // Default units.
    datamode = "advanced";// Default data mode (currently unused).
    forecastDays = 3;     // Default number of forecast days.
}

// --- Constructor ---

// Initializes preferences: loads defaults, then attempts to load from file.
Preferences::Preferences(const std::string& filename) : settingsFilename(filename) {
    loadDefaults(); // Establish baseline defaults.
    loadSettings(); // Override defaults with values from file, if successful.
}

// --- Getters ---
const std::string& Preferences::getApiKey() const { return apiKey; }
const std::string& Preferences::getLocation() const { return location; }
const std::string& Preferences::getUnits() const { return units; }
const std::string& Preferences::getDataMode() const { return datamode; }
int Preferences::getForecastDays() const { return forecastDays; }

// --- Setters ---

// Sets the API key directly.
void Preferences::setApiKey(const std::string& key) { apiKey = key; }
// Sets the location after trimming whitespace.
void Preferences::setLocation(const std::string& loc) { location = trimInternal(loc); }

// Sets the units if the provided string is valid ("Metric" or "Imperial").
// Returns true if set successfully, false otherwise (prints warning).
bool Preferences::setUnits(const std::string& unit) {
    std::string trimmedUnit = trimInternal(unit);
    if (trimmedUnit == "Metric" || trimmedUnit == "Imperial") {
        units = trimmedUnit;
        return true;
    }
    // Print warning if input is invalid, don't change the current value.
    std::cerr << "Warning: Invalid unit '" << unit << "' provided. Units remain '" << units << "'." << std::endl;
    return false;
}

// Sets the data mode after trimming whitespace.
void Preferences::setDataMode(const std::string& mode) { datamode = trimInternal(mode); }

// Sets the forecast days if the value is within the accepted range [1, 14].
// Returns true if set successfully, false otherwise (prints warning).
bool Preferences::setForecastDays(int days) {
    if (days >= 1 && days <= 14) { // WeatherAPI typically supports up to 14 days.
        forecastDays = days;
        return true;
    }
    // Print warning if input is invalid, don't change the current value.
    std::cerr << "Warning: Invalid forecast days value '" << days << "' (must be 1-14). Forecast days remain '" << forecastDays << "'." << std::endl;
    return false;
}

// --- File Operations ---

// Loads settings from the file specified by 'settingsFilename'.
// Parses lines in "key:value" format.
bool Preferences::loadSettings() {
    std::ifstream infile(settingsFilename);
    if (!infile.is_open()) {
        // Informative message if file doesn't exist or can't be opened.
        std::cerr << "Info: Could not open settings file '" << settingsFilename << "'. Using default/current settings." << std::endl;
        // Optionally, create a default file here on first run if desired: saveSettings();
        return false; // Indicate file wasn't read, though not necessarily an error state.
    }

    std::string line;
    bool loadedSomething = false; // Track if any valid settings were loaded.
    // Read file line by line.
    while (getline(infile, line)) {
        std::istringstream linestream(line);
        std::string key;
        // Split line at the first colon ':'.
        if (getline(linestream, key, ':')) {
            std::string value;
            getline(linestream, value); // Read the rest of the line as the value.
            std::string lowerKey = toLowerInternal(trimInternal(key)); // Normalize key.
            value = trimInternal(value); // Trim value.

            if (value.empty()) continue; // Skip lines with keys but no values.

            // Update corresponding preference based on the lowercase key.
            if (lowerKey == "apikey")       { setApiKey(value); loadedSomething = true; }
            else if (lowerKey == "location") { setLocation(value); loadedSomething = true; }
            else if (lowerKey == "units")    { if(setUnits(value)) loadedSomething = true; } // Use validating setter
            else if (lowerKey == "datamode") { setDataMode(value); loadedSomething = true; }
            else if (lowerKey == "forecastdays") {
                try {
                    int days = std::stoi(value); // Convert string value to int.
                    if(setForecastDays(days)) loadedSomething = true; // Use validating setter
                } catch (const std::invalid_argument&) {
                    std::cerr << "Warning: Invalid number format for 'forecastdays' ('" << value << "') in settings file." << std::endl;
                } catch (const std::out_of_range&) {
                     std::cerr << "Warning: Value for 'forecastdays' ('" << value << "') out of range in settings file." << std::endl;
                }
            }
            // Silently ignore unknown keys.
        }
    }
    infile.close(); // Close the file stream.

    if (loadedSomething) {
        std::cout << "Settings loaded successfully from '" << settingsFilename << "'" << std::endl;
    }
    // Return true indicating the load process completed (even if file was empty or only had warnings).
    return true;
}

// Saves the current preference values to the file specified by 'settingsFilename'.
// Overwrites the existing file content.
bool Preferences::saveSettings() const {
    std::ofstream outfile(settingsFilename);
    if (!outfile.is_open()) {
        std::cerr << "Error: Could not open settings file '" << settingsFilename << "' for saving." << std::endl;
        return false; // Indicate failure to open file.
    }

    // Write each setting in "key:value" format.
    outfile << "apikey:" << apiKey << std::endl;
    outfile << "location:" << location << std::endl;
    outfile << "units:" << units << std::endl;
    outfile << "datamode:" << datamode << std::endl;
    outfile << "forecastdays:" << forecastDays << std::endl;

    outfile.close(); // Close the file stream.

    // Check the stream state *after* closing to catch potential write errors.
    if (!outfile) {
         std::cerr << "Error: Failed to write all data to settings file '" << settingsFilename << "'." << std::endl;
         return false; // Indicate write failure.
    }

    std::cout << "Settings saved successfully to '" << settingsFilename << "'" << std::endl;
    return true; // Indicate successful save operation.
}