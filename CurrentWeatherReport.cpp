// CurrentWeatherReport.cpp
#include "CurrentWeatherReport.h"
#include <utility> // For std::move
#include <ostream> // For std::ostream

// Constructor - takes ownership of the Weather object by moving it
CurrentWeatherReport::CurrentWeatherReport(Weather conditions)
    : currentConditions(std::move(conditions)) {} // Use move constructor of Weather

std::string CurrentWeatherReport::getReportType() const {
  return "Current Conditions";
}

// Display implementation calls the helper on the contained Weather object
void CurrentWeatherReport::display(std::ostream& os) const {
  // Add a slightly clearer header maybe
  os << "\n=== " << getReportType() << " ===" << std::endl;
  currentConditions.displayData(os); // Call Weather's updated display helper
  os << "============================" << std::endl; // Matching footer
}

const Weather& CurrentWeatherReport::getWeather() const {
  return currentConditions;
}