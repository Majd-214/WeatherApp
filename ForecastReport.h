// ForecastReport.h
#ifndef FORECASTREPORT_H
#define FORECASTREPORT_H

#include "WeatherReport.h"
#include "Forecast.h" // Include Forecast definition
#include <memory>    // For std::unique_ptr or std::move
#include <string>

// Represents a report containing forecast data (daily/hourly)
class ForecastReport : public WeatherReport {
  public:
  // Enum to specify which forecast detail level to display
  enum class DetailLevel { DAILY, HOURLY };

  private:
  Forecast forecastData; // Contains the actual forecast data
  DetailLevel displayLevel; // Controls what detail is displayed

  public:
  // Constructor takes a Forecast object and the desired display level
  ForecastReport(Forecast data, DetailLevel level);

  // Override virtual functions from WeatherReport / IDisplayable
  std::string getReportType() const override;
  void display(std::ostream& os) const override;

  // Specific getter for this report type
  const Forecast& getForecast() const;

  private:
  // Private helpers for display logic, similar to old Forecast methods
  void displayDaily(std::ostream& os) const;
  void displayHourly(std::ostream& os) const;
};

#endif // FORECASTREPORT_H