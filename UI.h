// Ui.h
#ifndef WEATHER_UI_H
#define WEATHER_UI_H

#include "Weather.h"
#include "Forecast.h"
#include <iostream>

class UI {
public:
    static void displayWeather(const Weather& weather);
    static void displayMenu();
    static void clearConsole();
    static void pauseScreen();
    UI() = delete;
};

#endif