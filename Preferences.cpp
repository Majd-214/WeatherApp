// Preferences.cpp
#include "Preferences.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm> // for std::transform
#include <cctype>    // for ::tolower

// Helper function to convert string to lower case
string Preferences::toLower(const string& str) {
    string lowerStr = str;
    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    return lowerStr;
}

// Helper function to trim leading/trailing whitespace
string Preferences::trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\n\r\f\v");
    if (string::npos == first) {
        return str;
    }
    size_t last = str.find_last_not_of(" \t\n\r\f\v");
    return str.substr(first, (last - first + 1));
}


Preferences::Preferences()
    : apiKey(""), location("London"), units("Metric"), datamode("advanced"), forecastDays(3) {} // Default values

Preferences::Preferences(const string &key, const string &loc, const string &unit,
                         const string &mode, int days)
    : apiKey(key), location(loc), units(unit), datamode(mode), forecastDays(days) {}

bool Preferences::loadSettings(const char *filename) {
    ifstream infile(filename);

    if (!infile.is_open()) {
        cerr << "Warning: Could not open settings file '" << filename << "'. Using default settings." << endl;
        // Create a default settings file?
        // saveSettings(filename); // Optionally create one if it doesn't exist
        return false;
    }

    string line;
    while (getline(infile, line)) {
        istringstream linestream(line);
        string key;

        if (getline(linestream, key, ':')) {
            string value;
            // Read the rest of the line as the value
            getline(linestream, value);
            key = trim(key);
            value = trim(value);
            string lowerKey = toLower(key); // Use lowercase for comparison

            if (value.empty()) continue; // Skip lines without values

            // Standardize keys to lowercase for comparison
            if (lowerKey == "apikey") {
                apiKey = value;
            } else if (lowerKey == "location") {
                location = value;
            } else if (lowerKey == "units") {
                if (value == "Metric" || value == "Imperial") {
                     units = value;
                } else {
                    cerr << "Warning: Invalid unit '" << value << "' in settings file. Using default '" << units << "'." << endl;
                }
            } else if (lowerKey == "datamode") {
                datamode = value; // Assuming any string is valid for datamode for now
            } else if (lowerKey == "forecastdays") {
                try {
                    int days = stoi(value);
                    if (days >= 1 && days <= 14) { // Assuming 1-14 is the valid range
                         forecastDays = days;
                    } else {
                         cerr << "Warning: Invalid forecast days value '" << value << "' in settings file. Using default '" << forecastDays << "'." << endl;
                    }
                } catch (const std::invalid_argument& ia) {
                    cerr << "Warning: Invalid number format for forecastdays '" << value << "' in settings file. Using default '" << forecastDays << "'." << endl;
                } catch (const std::out_of_range& oor) {
                     cerr << "Warning: Forecastdays value '" << value << "' out of range in settings file. Using default '" << forecastDays << "'." << endl;
                }
            }
        }
    }
    infile.close();
    cout << "Settings loaded successfully from '" << filename << "'" << endl;
    return true;
}

bool Preferences::saveSettings(const char *filename) const {
    ofstream outfile(filename);

    if (!outfile.is_open()) {
        cerr << "Error: Could not open settings file '" << filename << "' for saving." << endl;
        return false;
    }

    // Use consistent lowercase keys for saving
    outfile << "apikey:" << apiKey << endl;
    outfile << "location:" << location << endl;
    outfile << "units:" << units << endl;
    outfile << "datamode:" << datamode << endl;
    outfile << "forecastdays:" << forecastDays << endl;

    outfile.close();

    if (!outfile) { // Check if any errors occurred during close/write
         cerr << "Error: Failed to write all data to settings file '" << filename << "'." << endl;
         return false;
    }
    cout << "Settings saved successfully to '" << filename << "'" << endl;
    return true;
}