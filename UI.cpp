// UI.cpp
#include "UI.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <cstdlib>
#include <ctime> // For time formatting

#ifdef _WIN32
#define CLEAR_COMMAND "cls"
#else
#define CLEAR_COMMAND "clear"
#endif

using namespace std;

void UI::displayWeather(const Weather& weather) {
    cout << "\n--- Current Weather Conditions ---" << endl;
    bool dataDisplayed = false;
    for (int i = 0; i < NUM_PROPERTIES; ++i) {
        Property* prop = weather.getProperty(static_cast<PropertyIndex>(i));
        if (prop != nullptr) {
            // Format Last Updated time
            if (static_cast<PropertyIndex>(i) == LAST_UPDATED) {
                 time_t epochTime = static_cast<time_t>(prop->getValue());
                 // Use std::put_time for safer formatting
                 std::tm timeinfo = {};
                 #ifdef _WIN32
                      localtime_s(&timeinfo, &epochTime);
                 #else
                      localtime_r(&epochTime, &timeinfo);
                 #endif
                 // Buffer for formatted time string
                 char time_buf[100];
                 // Format: YYYY-MM-DD HH:MM:SS Timezone Name (Timezone might be tricky/platform specific)
                 if (std::strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S %Z", &timeinfo)) {
                      cout << left << setw(15) << prop->getName() << ": " << time_buf << endl;
                 } else {
                      cout << left << setw(15) << prop->getName() << ": " << "(time formatting error)" << endl;
                 }

            } else { // Handle other properties
                 cout << left << setw(15) << prop->getName() << ": "
                      << fixed << setprecision(1) // Use 1 decimal place
                      << prop->getValue();
                 if (!prop->getUnit().empty()) {
                    cout << " " << prop->getUnit();
                 }
                 cout << endl;
            }
             dataDisplayed = true;
        }
    }
     if (!dataDisplayed) {
         cout << "No weather data available to display." << endl;
     }
     cout << "--------------------------------" << endl;
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
    cout << "Enter your choice: ";
}

void UI::clearConsole() {
    system(CLEAR_COMMAND);
}

void UI::pauseScreen() {
    cout << "\nPress Enter to continue...";
    // Clear potential leftover input from buffer before waiting
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    // If the ignore above consumes the 'Enter' press meant for pausing,
    // you might need cin.get() here to wait for a distinct press. Test it.
    // cin.get();
}