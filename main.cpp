// main.cpp - Main entry point for the Weather Application
#include "Preferences.h"       // Manages user settings (API key, location, units)
#include "APIConverter.h"      // Handles communication with the weather API
#include "UI.h"                // Provides console user interface functions
#include "WeatherReport.h"     // Abstract base class for reports
#include "CurrentWeatherReport.h" // Concrete report for current weather
#include "ForecastReport.h"    // Concrete report for forecast weather
#include "IDisplayable.h"      // Interface for displayable objects (used by UI)

#include <iostream> // For console input/output (cout, cerr)
#include <memory>   // For std::unique_ptr (manages report objects)
#include <string>   // For string manipulation

int main() {
    // --- Initialization Phase ---
    Preferences prefs; // Loads settings from "settings.txt" or uses defaults.

    // Ensure the API key is present, prompt user if missing.
    if (prefs.getApiKey().empty()) {
        std::cout << "API Key not found or empty in settings.txt." << std::endl;
        std::string key = UI::getTextInput("Please enter your WeatherAPI key: ");
        if (key.empty()) {
            std::cerr << "Error: API Key is required. Exiting." << std::endl;
            return 1; // Indicate failure
        }
        prefs.setApiKey(key); // Store the entered key
        prefs.saveSettings(); // Attempt to save it back to the file
    }

    // Create the API converter instance and configure it from preferences.
    APIConverter apiConverter; // Uses default WeatherAPI base URL
    apiConverter.setApiKey(prefs.getApiKey());
    apiConverter.setLocation(prefs.getLocation());
    apiConverter.setUnits(prefs.getUnits());

    // --- Main Application Loop ---
    int choice = 0;
    const int EXIT_CHOICE = 7; // Define the exit menu option number

    do {
        UI::clearConsole();          // Clear the screen for a fresh display
        UI::displayPreferences(prefs); // Show current settings
        UI::displayMenu();            // Show the main menu
        choice = UI::getMenuChoice(1, EXIT_CHOICE); // Get valid user input

        // Use a smart pointer to manage the dynamically created report object.
        std::unique_ptr<WeatherReport> report = nullptr;

        // Process the user's menu choice.
        switch (choice) {
            case 1: { // Get Current Weather - Braces optional here as no variables declared
                std::cout << "\nFetching Current Weather..." << std::endl;
                report = apiConverter.getCurrentWeather(); // Fetch and store report
                break;
            }
            case 2: { // Get Hourly Forecast - Braces optional here
                std::cout << "\nFetching Hourly Forecast..." << std::endl;
                report = apiConverter.getForecastReport(prefs.getForecastDays(), ForecastReport::DetailLevel::HOURLY);
                break;
            }
            case 3: { // Get Daily Forecast - Braces optional here
                std::cout << "\nFetching Daily Forecast Summary..." << std::endl;
                report = apiConverter.getForecastReport(prefs.getForecastDays(), ForecastReport::DetailLevel::DAILY);
                break;
            }
            case 4: { // Update Location - **ADDED BRACES**
                std::string newLocation = UI::getTextInput("Enter new location (e.g., City, zip, lat,lon): ");
                if (!newLocation.empty()) {
                    prefs.setLocation(newLocation);       // Update preferences object
                    apiConverter.setLocation(newLocation); // Update API converter state
                    std::cout << (prefs.saveSettings() ? "Location updated and saved." : "Location updated for session, but failed to save.") << std::endl;
                } else {
                    std::cout << "Location not changed (input was empty)." << std::endl;
                }
                UI::pauseScreen(); // Wait for user before continuing loop
                continue; // Skip report display for this iteration
            } // ** END BRACE **
            case 5: { // Update Units - **ADDED BRACES**
                 std::string newUnits = UI::getUnitsInput(); // Get validated "Metric" or "Imperial"
                 if (prefs.setUnits(newUnits)) { // Update prefs (validated)
                      apiConverter.setUnits(newUnits); // Update API converter state
                      std::cout << (prefs.saveSettings() ? "Units updated and saved." : "Units updated for session, but failed to save.") << std::endl;
                 } // else: setUnits already printed a warning
                 UI::pauseScreen();
                 continue; // Skip report display
            } // ** END BRACE **
             case 6: { // Update Forecast Days - **ADDED BRACES**
                int newDays = UI::getForecastDaysInput(); // Get validated 1-14
                if (prefs.setForecastDays(newDays)) { // Update prefs (validated)
                    // No direct update needed for apiConverter here, it reads days on request
                    std::cout << (prefs.saveSettings() ? "Forecast days updated and saved." : "Forecast days updated for session, but failed to save.") << std::endl;
                } // else: setForecastDays already printed a warning
                 UI::pauseScreen();
                 continue; // Skip report display
            } // ** END BRACE **
            case EXIT_CHOICE: { // Exit - Braces optional here
                std::cout << "Exiting Weather App..." << std::endl;
                continue; // Proceed to loop termination condition
            }
            default: { // Should not happen due to getMenuChoice validation - Braces optional here
                std::cerr << "Error: Invalid menu choice detected." << std::endl;
                 UI::pauseScreen();
                 continue;
            }
        } // End switch

        // --- Display Report (if a report was generated) ---
        if (report) {
             // Use the UI's display method, which leverages the IDisplayable interface
             // for polymorphic display of the specific report type.
            UI::displayReport(*report);
        } else if (choice != EXIT_CHOICE) {
             // If no report was generated (API call likely failed) and not exiting.
             std::cout << "\n*** Failed to retrieve or process the requested weather data. ***" << std::endl;
             std::cout << "*** Please check API Key, Location, and network connection. ***" << std::endl;
        }

        // Pause before looping back to the menu (unless exiting).
        if (choice != EXIT_CHOICE) {
             UI::pauseScreen();
        }

    } while (choice != EXIT_CHOICE); // Loop continues until user chooses to exit

    return 0; // Indicate successful execution
}