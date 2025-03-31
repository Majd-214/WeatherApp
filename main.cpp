#include "Preferences.h"
#include "Property.h"
#include "Weather.h"
#include "APIConverter.h"
#include "UI.h"
#include "Forecast.h"
#include <iostream>
#include <string>
#include <limits> // Required for numeric_limits

using namespace std;

int main() {
    Preferences preferences;

    if (!preferences.fileloading(("settings.txt")))
        cout << "Settings fileloading failed. Using default preferences." << endl;


    cout << "CurrentPreferences: " << endl;
    cout << "- Home Location: " << preferences.location << endl;
    cout << "- Units: " << preferences.units << endl;
    cout << "- Data mode: " << preferences.datamode << endl;

    APIConverter apiConverter("", "");
    string apiKey;
    string location;
    int forecastDays = 3;

    ifstream settingsFile("settings.txt");
    if (settingsFile.is_open()) {
        string line;
        while (getline(settingsFile, line)) {
            size_t delimiterPos = line.find(':');
            if (delimiterPos != string::npos) {
                string key = line.substr(0, delimiterPos);
                string value = line.substr(delimiterPos + 1);
                if (key == "apikey") {
                    apiKey = value;
                } else if (key == "location") {
                    location = value;
                } else if (key == "forecastdays") {
                    try {
                        forecastDays = stoi(value);
                        if (forecastDays > 14 || forecastDays < 1) {
                            cout << "Invalid forecast days in settings. Using default (3)." << endl;
                            forecastDays = 3;
                        }
                    } catch (const std::invalid_argument& e) {
                        cout << "Invalid forecast days format in settings. Using default (3)." << endl;
                    } catch (const std::out_of_range& e) {
                        cout << "Forecast days value out of range in settings. Using default (3)." << endl;
                    }
                }
            }
        }
        settingsFile.close();
    } else {
        cerr << "Unable to open settings.txt to read API key and location." << endl;
        return 1;
    }

    apiConverter.setApiKey(apiKey);
    apiConverter.setLocation(preferences.location);

    int choice;

    do {
        UI::clearConsole();
        UI::displayMenu();
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number between 1 and 4." << endl;
            continue;
        }

        switch (choice) {
        case 1: {
            Weather currentWeather;
            apiConverter.updateWeather(&currentWeather);
            UI::displayWeather(currentWeather);
            UI::pauseScreen();
            break;
        }
        case 2: {
            Forecast* forecastData = apiConverter.getForecast(forecastDays);
            if (forecastData) {
                forecastData->display();
                delete forecastData;
            } else {
                cout << "Failed to retrieve forecast data." << endl;
            }
            UI::pauseScreen();
            break;
        }
        case 3: {
            cout << "Enter new location: ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer
            getline(cin, location);
            apiConverter.setLocation(location);
            preferences.location = location;
            preferences.filesaving("settings.txt");
            cout << "Location updated." << endl;
            UI::pauseScreen();
            break;
        }
        case 4:
            cout << "Exiting Weather App..." << endl;
            break;
        default:
            cout << "Invalid choice. Please enter a number between 1 and 4." << endl;
            UI::pauseScreen();
        }
    } while (choice != 4);

    return 0;
}