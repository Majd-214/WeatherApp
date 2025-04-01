// ForecastReport.h
#ifndef FORECASTREPORT_H
#define FORECASTREPORT_H

#include "WeatherReport.h" // Base class interface
#include "Forecast.h"      // Contains the actual forecast data structure
#include <string>          // For getReportType return
#include <ostream>         // For display method parameters

// Concrete report class representing weather forecast data (daily or hourly).
// Inherits from WeatherReport and thus IDisplayable.
class ForecastReport : public WeatherReport {
  public:
  // Specifies the level of detail to be displayed by the report.
  enum class DetailLevel { DAILY, HOURLY };

  private:
  // Holds the underlying forecast data (multiple days).
  Forecast forecastData;
  // Determines whether to display daily summaries or hourly details.
  DetailLevel displayLevel;

  public:
  // Constructor: Takes ownership of the Forecast data via move and sets the display level.
  ForecastReport(Forecast data, DetailLevel level);

  // --- Overridden Virtual Methods ---

  // Returns the specific type identifier based on the DetailLevel.
  std::string getReportType() const override;
  // Implements the display logic, calling either displayDaily or displayHourly.
  void display(std::ostream& os) const override;

  // --- Specific Getter ---

  // Provides read-only access to the underlying Forecast data.
  const Forecast& getForecast() const;

  private:
  // --- Private Display Helpers ---

  // Formats and prints the daily forecast summaries.
  void displayDaily(std::ostream& os) const;
  // Formats and prints the hourly forecast details in a tabular format.
  void displayHourly(std::ostream& os) const;
};

#endif // FORECASTREPORT_H