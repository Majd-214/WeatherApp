// UI.h
#ifndef WEATHER_UI_H
#define WEATHER_UI_H

#include <iostream> // For std::ostream potentially
#include <string>   // For user input

// Forward declare interfaces/classes used
class IDisplayable;
class Preferences;

// Static class for handling User Interface elements
class UI {
    public:
    // Prevent instantiation
    UI() = delete;

    // --- Display Methods ---
    // Displays any object implementing IDisplayable (polymorphic)
    static void displayReport(const IDisplayable& report);

    // Displays the main menu options
    static void displayMenu();

    // Displays current preference settings
    static void displayPreferences(const Preferences& prefs);

    // --- Console Utilities ---
    static void clearConsole();
    static void pauseScreen();

    // --- User Input ---
    // Gets a validated integer choice from the user
    static int getMenuChoice(int minChoice, int maxChoice);
    // Gets a line of text input
    static std::string getTextInput(const std::string& prompt);
    // Gets validated forecast days input
    static int getForecastDaysInput();
    // Gets validated units input
    static std::string getUnitsInput();
};

#endif // WEATHER_UI_H