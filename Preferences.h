// Preferences.h
#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <string>
#include <fstream> // Keep for file operations if needed directly
#include <vector>  // Keep if used

// Class to manage user preferences, now fully encapsulated
class Preferences {
  private:
  std::string apiKey;
  std::string location;
  std::string units; // "Metric" or "Imperial"
  std::string datamode; // Example: "basic", "advanced" (if used)
  int forecastDays; // Number of days for forecast (e.g., 1-14)
  std::string settingsFilename; // Store the filename

  // Private helpers
  std::string trim(const std::string& str);
  std::string toLower(const std::string& str);
  void loadDefaults(); // Helper to set default values

  public:
  // Constructor uses a default filename
  Preferences(const std::string& filename = "settings.txt");

  // Prevent copying and assignment (or implement properly if needed)
  Preferences(const Preferences&) = delete;
  Preferences& operator=(const Preferences&) = delete;

  // --- Getters ---
  const std::string& getApiKey() const;
  const std::string& getLocation() const;
  const std::string& getUnits() const;
  const std::string& getDataMode() const;
  int getForecastDays() const;

  // --- Setters (with validation where applicable) ---
  void setApiKey(const std::string& key);
  void setLocation(const std::string& loc);
  // Returns true if unit was valid and set, false otherwise
  bool setUnits(const std::string& unit);
  void setDataMode(const std::string& mode);
  // Returns true if days were valid and set, false otherwise
  bool setForecastDays(int days);

  // --- File Operations ---
  // Load settings from the stored filename
  bool loadSettings();
  // Save settings to the stored filename
  bool saveSettings() const;
};

#endif // PREFERENCES_H