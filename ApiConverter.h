// APIConverter.h
#ifndef APICONVERTER_H
#define APICONVERTER_H

#include <string>
#include <memory> // For std::unique_ptr

#include "ForecastReport.h"

// Forward declarations to reduce header dependencies
class Weather;
class Forecast;
class CurrentWeatherReport;
class ForecastReport;
namespace httplib { class Client; } // Forward declare library class

// Converts API responses into WeatherReport objects
class APIConverter {
    private:
    // Using unique_ptr for composition/ownership of the client
    std::unique_ptr<httplib::Client> client;
    std::string apiKey;
    std::string location;
    std::string units; // "Metric" or "Imperial"

    public:
    // Constructor now takes client details (e.g., base URL)
    explicit APIConverter(const std::string& apiBaseUrl = "http://api.weatherapi.com");
    ~APIConverter(); // Needed because of unique_ptr to incomplete type in header

    // Prevent copying - managing resources like http client
    APIConverter(const APIConverter&) = delete;
    APIConverter& operator=(const APIConverter&) = delete;

    // --- Configuration ---
    void setApiKey(const std::string& key);
    void setLocation(const std::string& loc);
    // Returns true if unit was valid and set, false otherwise
    bool setUnits(const std::string& unit);

    // --- Getters (Optional - consider if needed externally) ---
    // const std::string& getApiKey() const;
    // const std::string& getLocation() const;
    // const std::string& getUnits() const;

    // --- API Interaction ---
    // Fetches current weather, returns a report object (or nullptr on failure)
    std::unique_ptr<CurrentWeatherReport> getCurrentWeather();

    // Fetches forecast, returns a report object (or nullptr on failure)
    std::unique_ptr<ForecastReport> getForecastReport(int days, ForecastReport::DetailLevel detail);
};

#endif // APICONVERTER_H