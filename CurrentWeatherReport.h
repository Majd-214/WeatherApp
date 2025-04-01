// CurrentWeatherReport.h
#ifndef CURRENTWEATHERREPORT_H
#define CURRENTWEATHERREPORT_H

#include "WeatherReport.h"
#include "Weather.h" // Include Weather definition
#include <memory> // For std::unique_ptr or std::move

// Represents a report containing current weather conditions
class CurrentWeatherReport : public WeatherReport {
  private:
  Weather currentConditions; // Contains the actual weather data

  public:
  // Constructor takes a Weather object (moves or copies)
  explicit CurrentWeatherReport(Weather conditions);

  // Override virtual functions from WeatherReport / IDisplayable
  std::string getReportType() const override;
  void display(std::ostream& os) const override;

  // Specific getter for this report type
  const Weather& getWeather() const;
};

#endif // CURRENTWEATHERREPORT_H