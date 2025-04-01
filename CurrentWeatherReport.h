// CurrentWeatherReport.h
#ifndef CURRENTWEATHERREPORT_H
#define CURRENTWEATHERREPORT_H

#include "WeatherReport.h" // Base class interface
#include "Weather.h"       // Contains the actual weather data structure

#include <string>    // For getReportType return
#include <ostream>   // For display method parameter

// Concrete report class representing current weather conditions.
// Inherits from WeatherReport and thus IDisplayable.
class CurrentWeatherReport : public WeatherReport {
  private:
  // Holds the weather data specific to the current conditions.
  Weather currentConditions;

  public:
  // Constructor: Takes ownership of the provided Weather data via move.
  explicit CurrentWeatherReport(Weather conditions);

  // --- Overridden Virtual Methods ---

  // Returns the specific type identifier for this report.
  std::string getReportType() const override;
  // Implements the display logic for current weather using the contained Weather object.
  void display(std::ostream& os) const override;

  // --- Specific Getter ---

  // Provides read-only access to the underlying Weather data.
  const Weather& getWeather() const;
};

#endif // CURRENTWEATHERREPORT_H