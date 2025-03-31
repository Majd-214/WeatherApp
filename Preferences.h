// Preferences.h
#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <string>
#include <fstream> // Added for friend declaration

using namespace std;

class Preferences {
public:
    string apiKey; // Added apiKey member
    string location;
    string units;
    string datamode;
    int forecastDays;

    Preferences();
    Preferences(const string &apiKey, const string &location, const string &units,
                const string &datamode, int forecastDays);

    // Made filenames const char* for broader compatibility, though string is fine too
    bool loadSettings(const char *filename = "settings.txt");
    bool saveSettings(const char *filename = "settings.txt") const;

private:
    // Helper to trim whitespace
    string trim(const string& str);
    // Helper to convert string to lower case for case-insensitive key matching
    string toLower(const string& str);
};

#endif // PREFERENCES_H