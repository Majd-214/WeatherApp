// main.cpp
#include "Preferences.h"       // For managing settings
#include "APIConverter.h"      // For fetching data
#include "UI.h"                // For user interaction
#include "WeatherReport.h"     // For report base class
#include "CurrentWeatherReport.h" // For specific report types
#include "ForecastReport.h"
#include "IDisplayable.h"      // For the display interface

#include <iostream>
#include <memory> // For std::unique_ptr
#include <string>

int main() {
    // --- Initialization ---
    Preferences prefs; // Loads settings automatically in constructor

    // Ensure API key exists
    if (prefs.getApiKey().empty()) {
        std::cout << "API Key not found or is empty in settings.txt." << std::endl;
        std::string key = UI::getTextInput("Please enter your WeatherAPI key: ");
        if (key.empty()) {
            std::cerr << "Error: API Key is required. Exiting." << std::endl;
            return 1;
        }
        prefs.setApiKey(key);
        prefs.saveSettings(); // Save the entered key
    }

    // Create API Converter
    APIConverter apiConverter; // Uses default base URL
    apiConverter.setApiKey(prefs.getApiKey()); // Set initial config
    apiConverter.setLocation(prefs.getLocation());
    apiConverter.setUnits(prefs.getUnits());

    // --- Main Application Loop ---
    int choice = 0;
    const int EXIT_CHOICE = 7;

    do {
        UI::clearConsole();
        UI::displayPreferences(prefs); // Show current settings
        UI::displayMenu();            // Show menu options
        choice = UI::getMenuChoice(1, EXIT_CHOICE); // Get validated choice
        std::unique_ptr<WeatherReport> report = nullptr; // Smart pointer for reports

        switch (choice) {
            case 1: { // Get Current Weather
                std::cout << "\nFetching Current Weather..." << std::endl;
                // APIConverter now returns the report directly
                report = apiConverter.getCurrentWeather();
                break;
            }
            case 2: { // Get Hourly Forecast
                std::cout << "\nFetching Hourly Forecast..." << std::endl;
                report = apiConverter.getForecastReport(prefs.getForecastDays(), ForecastReport::DetailLevel::HOURLY);
                break;
            }
            case 3: { // Get Daily Forecast
                std::cout << "\nFetching Daily Forecast Summary..." << std::endl;
                 report = apiConverter.getForecastReport(prefs.getForecastDays(), ForecastReport::DetailLevel::DAILY);
                break;
            }
            case 4: { // Update Location
                std::string newLocation = UI::getTextInput("Enter new location: ");
                if (!newLocation.empty()) {
                    prefs.setLocation(newLocation);
                    apiConverter.setLocation(newLocation); // Update converter too
                    if (prefs.saveSettings()) {
                        std::cout << "Location updated and saved." << std::endl;
                    } else { std::cout << "Location updated for session, but failed to save." << std::endl; }
                } else { std::cout << "Location not changed (input was empty)." << std::endl; }
                UI::pauseScreen();
                continue; // Skip report display for this option
            }
            case 5: { // Update Units
                 std::string newUnits = UI::getUnitsInput(); // Gets validated input
                 if (prefs.setUnits(newUnits)) { // Use setter validation
                      apiConverter.setUnits(newUnits); // Update converter too
                      if (prefs.saveSettings()) {
                           std::cout << "Units updated and saved." << std::endl;
                      } else { std::cout << "Units updated for session, but failed to save." << std::endl; }
                 } // Setter already prints warning if invalid
                 UI::pauseScreen();
                 continue; // Skip report display
            }
             case 6: { // Update Forecast Days
                int newDays = UI::getForecastDaysInput(); // Gets validated input
                if (prefs.setForecastDays(newDays)) { // Use setter validation
                    // No need to update apiConverter unless getForecast uses it differently
                    if (prefs.saveSettings()) {
                        std::cout << "Forecast days updated and saved." << std::endl;
                    } else { std::cout << "Forecast days updated for session, but failed to save." << std::endl; }
                } // Setter already prints warning if invalid
                 UI::pauseScreen();
                 continue; // Skip report display
            }
            case EXIT_CHOICE: {
                std::cout << "Exiting Weather App..." << std::endl;
                continue; // Exit loop
            }
            default: { // Should not happen with getMenuChoice validation
                std::cerr << "Error: Invalid menu choice detected." << std::endl;
                 UI::pauseScreen();
                 continue;
            }
        } // End switch

        // --- Display Report (if one was generated) ---
        if (report) {
             // Polymorphic display using the IDisplayable interface
            UI::displayReport(*report);
        } else {
             // API call failed, error messages should have been printed by APIConverter
             std::cout << "\n*** Failed to retrieve or process the requested weather data. ***" << std::endl;
             std::cout << "*** Please check API Key, Location, and network connection. ***" << std::endl;
        }
         UI::pauseScreen(); // Pause after displaying report or failure message

    } while (choice != EXIT_CHOICE);

    return 0;
}