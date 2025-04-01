// UI.h
#ifndef WEATHER_UI_H
#define WEATHER_UI_H

#include <string>   // For prompt strings and input/output

// Forward declarations of classes used by UI functions (reduces header dependencies).
class IDisplayable; // Interface for objects that can be displayed.
class Preferences;  // Class holding application settings.

// Provides static methods for handling console-based User Interface interactions.
// Designed as a utility class (no instances needed).
class UI {
    public:
    // Delete constructor to prevent instantiation of this utility class.
    UI() = delete;

    // --- Display Methods ---

    // Displays any object that conforms to the IDisplayable interface.
    // Uses polymorphism to call the correct display() method.
    static void displayReport(const IDisplayable& report);

    // Displays the main application menu to the console.
    static void displayMenu();

    // Displays the current settings stored in the Preferences object.
    static void displayPreferences(const Preferences& prefs);

    // --- Console Utility Methods ---

    // Clears the console screen (platform-dependent implementation).
    static void clearConsole();

    // Pauses execution and waits for the user to press Enter (platform-dependent).
    static void pauseScreen();

    // --- User Input Methods ---

    // Prompts the user and gets a validated integer choice within a specified range.
    static int getMenuChoice(int minChoice, int maxChoice);

    // Prompts the user and gets a line of text input.
    static std::string getTextInput(const std::string& prompt);

    // Prompts the user and gets a validated integer for forecast days (1-14).
    static int getForecastDaysInput();

    // Prompts the user and gets validated units input ("Metric" or "Imperial").
    static std::string getUnitsInput();
};

#endif // WEATHER_UI_H