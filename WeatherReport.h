// WeatherReport.h
#ifndef WEATHERREPORT_H
#define WEATHERREPORT_H

#include "IDisplayable.h" // Inherits the display interface
#include <string>         // For getReportType return value

// Abstract base class for all types of weather reports (e.g., Current, Forecast).
// Ensures all reports are displayable and can identify their type.
class WeatherReport : public IDisplayable {
  public:
  // Virtual destructor: Ensures derived class destructors are called correctly
  // when deleting through a base class pointer. Default implementation is sufficient here.
  ~WeatherReport() override = default;

  // Pure virtual function: Derived classes MUST provide an implementation
  // to return a string identifying the specific report type (e.g., "Current Conditions").
  virtual std::string getReportType() const = 0;

  // The pure virtual 'display' method is inherited from IDisplayable.
  // Derived classes must also implement:
  // virtual void display(std::ostream& os) const override = 0;
};

#endif // WEATHERREPORT_H