// UI.cpp
#include "UI.h"
#include "IDisplayable.h" // Needed for displayReport parameter type
#include "Preferences.h"  // Needed for displayPreferences parameter type
#include <iostream>       // For console I/O (cout, cin)
#include <limits>         // For numeric_limits (used in input validation)
#include <string>         // For string manipulation
#include <cstdlib>        // For system() calls (clear, pause) - Platform dependent

// Use standard namespace for convenience.
using namespace std;

// --- Display Methods ---

// Displays a report by leveraging the overloaded << operator for IDisplayable.
void UI::displayReport(const IDisplayable& report) {
    cout << report; // Polymorphic call to the report's display() method via operator<<
}

// Prints the main menu options to the console.
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

// Prints the current preference settings from the Preferences object.
void UI::displayPreferences(const Preferences& prefs) {
     cout << "--- Current Settings ---" << endl;
     cout << "Location:      " << prefs.getLocation() << endl;
     cout << "Units:         " << prefs.getUnits() << endl;
     cout << "Forecast Days: " << prefs.getForecastDays() << endl;
     // Indicate whether the API key has been set (without displaying the key itself).
     cout << "API Key Set:   " << (prefs.getApiKey().empty() ? "No" : "Yes") << endl;
     cout << "------------------------" << endl;
}

// --- Console Utilities ---

// Clears the console screen using platform-specific commands.
void UI::clearConsole() {
    // Ensure output buffer is flushed before calling system command.
    cout << flush;
#ifdef _WIN32 // Windows-specific command
    system("CLS");
#else // Assumed POSIX-compliant (Linux, macOS) - uses ANSI escape or 'clear' command.
    // system("printf '\\33c'"); // Alternative using ANSI reset code
    system("clear"); // More common POSIX command
#endif
}

// Pauses execution and waits for user input (Enter key).
void UI::pauseScreen() {
    // Ensure prompt message is displayed before pausing.
    cout << "\nPress Enter to continue..." << flush;
    // Clear the input buffer before waiting for Enter.
    // Handles potential leftover newline characters from previous input.
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    // cin.get(); // Alternative: Waits for any character then Enter. Use if ignore doesn't wait.
}

// --- User Input ---

// Gets an integer menu choice from the user, validating the input range.
int UI::getMenuChoice(int minChoice, int maxChoice) {
    int choice = 0;
    cout << "Enter your choice (" << minChoice << "-" << maxChoice << "): ";

    // Loop until valid integer input within the specified range is received.
    while (!(cin >> choice) || choice < minChoice || choice > maxChoice) {
        cout << "Invalid input. Please enter a number between " << minChoice << " and " << maxChoice << ": ";
        cin.clear(); // Clear error flags on cin.
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input buffer.
    }
    // Clear any remaining characters in the input buffer (like the newline after the number).
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return choice;
}

// Gets a line of text input from the user after displaying a prompt.
string UI::getTextInput(const string& prompt) {
    string input;
    cout << prompt;
    getline(cin, input); // Reads the entire line, including spaces.
    return input;
}

// Gets the desired number of forecast days from the user, validating the range [1, 14].
int UI::getForecastDaysInput() {
     int days = 0;
     cout << "Enter new number of forecast days (1-14): ";
     // Loop until valid integer input within the range [1, 14] is received.
      while (!(cin >> days) || days < 1 || days > 14) {
        cout << "Invalid input. Please enter a number between 1 and 14: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear trailing newline.
    return days;
}

// Gets the desired unit system ("Metric" or "Imperial") from the user, ensuring valid input.
string UI::getUnitsInput() {
    string unit;
    cout << "Enter new unit (Metric or Imperial): ";
    // Loop until either "Metric" or "Imperial" is entered (case-sensitive).
    while (getline(cin, unit) && unit != "Metric" && unit != "Imperial") {
          cout << "Invalid unit. Please enter 'Metric' or 'Imperial': ";
          // No need for cin.clear() or cin.ignore() here as getline handles line consumption.
     }
    return unit;
}