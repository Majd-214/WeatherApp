// CurrentWeatherReport.cpp
#include "CurrentWeatherReport.h"
#include <utility> // For std::move
#include <ostream> // For std::ostream parameter in display

// Constructor: Initializes the member 'currentConditions' by moving the input 'conditions'.
CurrentWeatherReport::CurrentWeatherReport(Weather conditions)
    : currentConditions(std::move(conditions)) {} // Efficiently transfers ownership

// Returns a string identifying this report type.
std::string CurrentWeatherReport::getReportType() const {
  return "Current Conditions";
}

// Displays the current weather report to the output stream.
void CurrentWeatherReport::display(std::ostream& os) const {
  // Print a header for clarity.
  os << "\n=== " << getReportType() << " ===" << std::endl;
  // Delegate the detailed data formatting to the Weather object's helper method.
  currentConditions.displayData(os);
  // Print a matching footer.
  os << "============================" << std::endl;
}

// Provides const access to the contained Weather data object.
const Weather& CurrentWeatherReport::getWeather() const {
  return currentConditions;
}