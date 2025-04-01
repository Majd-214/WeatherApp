// Preferences.h
#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <string> // For storing preference values
#include <fstream> // Used in .cpp for file I/O, not needed here directly

// Manages user preferences and application settings, handling loading/saving from a file.
class Preferences {
private:
    // Application settings variables.
    std::string apiKey;
    std::string location;
    std::string units;       // Expected: "Metric" or "Imperial"
    std::string datamode;    // Currently unused setting ("basic", "advanced")
    int forecastDays;        // Number of days for forecast (e.g., 1-14)

    // File handling variable.
    std::string settingsFilename; // Name of the file to load/save settings.

    // --- Private Helper Methods (Implementation in .cpp) ---

    // Trims leading/trailing whitespace from a string.
    std::string trim(const std::string& str);
    // Converts a string to lowercase.
    std::string toLower(const std::string& str);
    // Sets the member variables to their default values.
    void loadDefaults();

public:
    // Constructor: Initializes defaults and attempts to load from the specified file.
    Preferences(const std::string& filename = "settings.txt");

    // Disable copy operations to prevent unintended side effects with file state.
    Preferences(const Preferences&) = delete;
    Preferences& operator=(const Preferences&) = delete;

    // --- Getters (Provide read-only access to settings) ---
    const std::string& getApiKey() const;
    const std::string& getLocation() const;
    const std::string& getUnits() const;
    const std::string& getDataMode() const; // Although unused, keep getter if defined
    int getForecastDays() const;

    // --- Setters (Allow modification of settings, with validation) ---

    // Sets the API key.
    void setApiKey(const std::string& key);
    // Sets the location (trims input).
    void setLocation(const std::string& loc);
    // Sets the units if valid ("Metric" or "Imperial"), returns success status.
    bool setUnits(const std::string& unit);
    // Sets the data mode (trims input).
    void setDataMode(const std::string& mode);
    // Sets the forecast days if within valid range (1-14), returns success status.
    bool setForecastDays(int days);

    // --- File Operations ---

    // Attempts to load settings from the 'settingsFilename'.
    // Returns true on success (even if file not found/empty), false on read error.
    bool loadSettings();
    // Attempts to save the current settings to 'settingsFilename'.
    // Returns true on success, false on write error.
    bool saveSettings() const;
};

#endif // PREFERENCES_H