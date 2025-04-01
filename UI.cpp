// UI.cpp
#include "UI.h"
#include "IDisplayable.h"
#include "Preferences.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <string>
#include <cstdlib> // Still potentially useful for other system() calls if any

using namespace std;

// --- Display Methods ---
// (displayReport, displayMenu, displayPreferences remain the same as previous version)
void UI::displayReport(const IDisplayable& report) {
    cout << report;
}

void UI::displayMenu() {
    cout << "\n=== Weather App Menu ===" << endl;
    cout << "1. Get Current Weather" << endl;
    cout << "2. Get Hourly Forecast" << endl;
    cout << "3. Get Daily Forecast" << endl;
    cout << "4. Update Location" << endl;
    cout << "5. Update Units (Metric/Imperial)" << endl;
    cout << "6. Update Forecast Days (1-14)" << endl;
    cout << "7. Exit" << endl;
    cout << "========================" << endl;
}

void UI::displayPreferences(const Preferences& prefs) {
     cout << "--- Preferences Currently ---" << endl;
     cout << "Location:      " << prefs.getLocation() << endl;
     cout << "Units:         " << prefs.getUnits() << endl;
     cout << "Forecast Days: " << prefs.getForecastDays() << endl;
     cout << "API Key Set:   " << (prefs.getApiKey().empty() ? "No" : "Yes") << endl;
     cout << "---------------------------" << endl;
}


// --- Console Utilities ---
void UI::clearConsole() {
    cout << flush;
#ifdef _WIN32
    system("CLS");
#else
    // Use ANSI escape codes as fallback for non-Windows (Linux, macOS)
    // This might still fail in some IDE consoles but works in standard terminals
    system("clear");
#endif
}

void UI::pauseScreen() {
#ifdef _WIN32
    // Flush cout before calling system("PAUSE")
    system("PAUSE");
    // PAUSE already waits for Enter, so no cin.ignore/get needed here.
#else
    cout << "\nPress Enter to continue..." << flush << endl;
    // Use cin.ignore/get for non-Windows platforms
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    // cin.get(); // Uncomment if ignore consumes the Enter press
#endif
}

// --- User Input ---
// (getMenuChoice, getTextInput, getForecastDaysInput, getUnitsInput remain the same)
int UI::getMenuChoice(int minChoice, int maxChoice) {
    int choice = 0;
    cout << "Enter your choice (" << minChoice << "-" << maxChoice << "): ";
    cin >> choice;

    while (cin.fail() || choice < minChoice || choice > maxChoice) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a number between " << minChoice << " and " << maxChoice << ": ";
        cin >> choice;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return choice;
}

string UI::getTextInput(const string& prompt) {
    string input;
    cout << prompt;
    getline(cin, input);
    return input;
}

int UI::getForecastDaysInput() {
     int days = 0;
     cout << "Enter new number of forecast days (1-14): ";
     cin >> days;
      while (cin.fail() || days < 1 || days > 14) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a number between 1 and 14: ";
        cin >> days;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return days;
}

string UI::getUnitsInput() {
    string unit;
    cout << "Enter new unit (Metric or Imperial): ";
    getline(cin, unit);
     while (unit != "Metric" && unit != "Imperial") {
          cout << "Invalid unit. Please enter 'Metric' or 'Imperial': ";
          getline(cin, unit);
     }
    return unit;
}