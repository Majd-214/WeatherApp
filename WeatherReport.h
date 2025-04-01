// WeatherReport.h
#ifndef WEATHERREPORT_H
#define WEATHERREPORT_H

#include "IDisplayable.h"
#include <string>
#include <memory> // For std::unique_ptr (potential future use)

// Abstract base class for different kinds of weather reports
class WeatherReport : public IDisplayable {
  public:
  // Ensure derived class destructors are called correctly
  ~WeatherReport() override = default;

  // Pure virtual function to get the specific report type
  virtual std::string getReportType() const = 0;

  // display method inherited from IDisplayable remains pure virtual here
  // or could be implemented to provide common formatting if desired.
  // void display(std::ostream& os) const override = 0;
};

#endif // WEATHERREPORT_H