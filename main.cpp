// main.cpp
#include "Preferences.h"
#include "Property.h"
#include "Weather.h"
#include "APIConverter.h"
#include "UI.h"
#include "Forecast.h"
#include <iostream>
#include <string>
#include <limits>
#include <sstream>

using namespace std;

int main() {
    Preferences preferences;
    preferences.loadSettings("settings.txt");

    if (preferences.apiKey.empty()) {
        cout << "API Key not found in settings.txt or is empty." << endl;
        cout << "Please enter your WeatherAPI key: ";
        getline(cin, preferences.apiKey);
        preferences.saveSettings("settings.txt"); // Save immediately if entered
        // Add a check here if the key is *still* empty after trying to read it.
        if (preferences.apiKey.empty()) {
             cerr << "Error: API Key is required to run the application. Exiting." << endl;
             return 1; // Exit if no API key can be obtained
        }
        cout << "API Key saved. Please restart the application if you were prompted." << endl;
        // It might be better to just exit here and ask user to restart
        // return 0;
    }


    APIConverter apiConverter(preferences.apiKey, preferences.location);
    apiConverter.setUnits(preferences.units);

    int choice = 0; // Initialize choice

    do {
        UI::clearConsole(); // Clear screen at the beginning of the loop

        // Display current preferences
        cout << "Preferences Currently:" << endl;
        cout << "Location: " << preferences.location << endl;
        cout << "Units: " << preferences.units << endl;
        // cout << "Data mode: " << preferences.datamode << endl; // Uncomment if datamode is used
        cout << "Forecast Days: " << preferences.forecastDays << endl;
        cout << "API Key Loaded: Yes" << endl; // Key must be loaded by now


        // Display main menu
        UI::displayMenu();
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number." << endl;
            choice = 0; // Reset choice to avoid unintended exit or loop break
            UI::pauseScreen(); // Pause after invalid input message
            continue;
        }
        // Consume the newline character left by cin >> choice
        // Do this *only* if input succeeded
        cin.ignore(numeric_limits<streamsize>::max(), '\n');


        switch (choice) {
        case 1: { // Current Weather
            cout << "\nFetching Current Weather..." << endl;
            Weather currentWeather;
            apiConverter.setApiKey(preferences.apiKey); // Ensure converter has the latest settings
            apiConverter.setLocation(preferences.location);
            apiConverter.setUnits(preferences.units);
            apiConverter.updateWeather(&currentWeather); // This function prints errors internally
            UI::displayWeather(currentWeather); // Display whatever was fetched (even if empty)
            UI::pauseScreen(); // Pause after displaying
            break;
        }
        case 2: // Hourly Forecast
        case 3: // Daily Forecast
        {
            cout << "\nFetching " << (choice == 2 ? "Hourly" : "Daily") << " Forecast for " << preferences.forecastDays << " days..." << endl;
            apiConverter.setApiKey(preferences.apiKey); // Ensure converter has the latest settings
            apiConverter.setLocation(preferences.location);
            apiConverter.setUnits(preferences.units);

            Forecast* forecastData = apiConverter.getForecast(preferences.forecastDays); // This prints errors internally if null

            if (forecastData) {
                 cout << "Forecast data received. Displaying..." << endl;
                if (choice == 2) {
                    forecastData->displayHourly();
                } else {
                    forecastData->displayDaily();
                }
                delete forecastData;
            } else {
                // This message might be redundant if getForecast already printed specific errors
                cout << "\n*** Failed to retrieve or process forecast data. ***" << endl;
                cout << "*** Please check API Key, Location (" << preferences.location << "), and network connection. ***" << endl;
            }
             UI::pauseScreen(); // Pause after displaying forecast OR the failure message
            break;
        }
        case 4: { // Update Location
            cout << "Enter new location: ";
            getline(cin, preferences.location); // Use getline after cin >> number
             if (preferences.location.empty()) {
                 cout << "Location cannot be empty." << endl;
             } else {
                apiConverter.setLocation(preferences.location);
                if (preferences.saveSettings("settings.txt")) {
                    cout << "Location updated and saved." << endl;
                } else {
                     cout << "Location updated for this session, but failed to save." << endl;
                }
             }
            UI::pauseScreen();
            break;
        }
        case 5: { // Update Units
            cout << "Enter new unit (Metric or Imperial): ";
            string newUnit;
            getline(cin, newUnit); // Use getline
            if (newUnit == "Metric" || newUnit == "Imperial") {
                preferences.units = newUnit;
                apiConverter.setUnits(preferences.units);
                if (preferences.saveSettings("settings.txt")) {
                    cout << "Units updated and saved." << endl;
                } else {
                    cout << "Units updated for this session, but failed to save." << endl;
                }
            } else {
                cout << "Invalid unit. Please enter 'Metric' or 'Imperial'." << endl;
            }
            UI::pauseScreen();
            break;
        }
        case 6: { // Update Forecast Days
            cout << "Enter new number of forecast days (1-14): ";
            int newForecastDays;
            cin >> newForecastDays;

            if (cin.fail()) {
                 cin.clear();
                 cin.ignore(numeric_limits<streamsize>::max(), '\n');
                 cout << "Invalid input. Please enter a number." << endl;
            } else {
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Consume newline
                if (newForecastDays >= 1 && newForecastDays <= 14) {
                    preferences.forecastDays = newForecastDays;
                    if (preferences.saveSettings("settings.txt")) {
                        cout << "Forecast days updated and saved." << endl;
                    } else {
                         cout << "Forecast days updated for this session, but failed to save." << endl;
                    }
                } else {
                    cout << "Invalid number of days. Please enter a value between 1 and 14." << endl;
                }
            }
            UI::pauseScreen();
            break;
        }
        case 7:
            cout << "Exiting Weather App..." << endl;
            break;
        default:
            cout << "Invalid choice (" << choice << "). Please try again." << endl;
            UI::pauseScreen(); // Pause after invalid choice message
        }
    } while (choice != 7);

    return 0;
}