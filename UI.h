// UI.h
#ifndef WEATHER_UI_H
#define WEATHER_UI_H

#include "Weather.h"
#include "Forecast.h"
#include <iostream>

class UI { // Static Class
  public:
  static void displayWeather(const Weather& weather);
  static void displayMenu();
  static void clearConsole();
  static void pauseScreen();
  UI() = delete; // Ensures no object of type UI can be created to keep static property
};

#endif