// Ui.cpp
#include "UI.h"
#include <iostream>
#include <iomanip>
#include <limits> // Required for numeric_limits
#ifdef _WIN32
#include <windows.h>
#else
#include <cstdlib>
#include <unistd.h>
#endif

using namespace std;

void UI::displayWeather(const Weather& weather) {
    cout << "\nCurrent Weather Conditions:" << endl;
    for (int i = 0; i < NUM_PROPERTIES; ++i) {
        Property* prop = weather.getProperty(static_cast<PropertyIndex>(i));
        if (prop != nullptr) {
            cout << "- " << left << setw(15) << prop->getName() << ": " << fixed << setprecision(2) << prop->getValue();
            if (!prop->getUnit().empty())
                cout << " " << prop->getUnit();
            cout << endl;
        }
    }
}

void UI::displayMenu() {
    cout << "\nWeather App Menu:" << endl;
    cout << "1. Get Current Weather" << endl;
    cout << "2. Get Forecast" << endl;
    cout << "3. Update Location" << endl;
    cout << "4. Exit" << endl;
    cout << "Enter your choice: ";
}

void UI::clearConsole() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void UI::pauseScreen() {
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}