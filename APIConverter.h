// ApiConverter.h
#ifndef APICONVERTER_H
#define APICONVERTER_H

#include <string>
#include <memory> // For std::unique_ptr

// Forward declarations to minimize header dependencies
#include "ForecastReport.h" // Needed for DetailLevel enum definition
namespace httplib { class Client; } // Forward declare external library class
class CurrentWeatherReport;
// class ForecastReport; // Already included for DetailLevel

// Handles interaction with the weather API, fetching data and converting it
// into report objects (CurrentWeatherReport, ForecastReport).
class APIConverter {
private:
    // Manages the HTTP client connection using a smart pointer.
    std::unique_ptr<httplib::Client> client;
    // API key for authentication.
    std::string apiKey;
    // Target location for weather data (e.g., "City", "lat,lon").
    std::string location;
    // Units for retrieved data ("Metric" or "Imperial").
    std::string units;

public:
    // Constructor: Initializes the HTTP client with the base API URL.
    explicit APIConverter(const std::string& apiBaseUrl = "http://api.weatherapi.com");
    // Destructor: Necessary for proper cleanup of the unique_ptr.
    ~APIConverter();

    // Disable copy operations to prevent issues with resource management (client).
    APIConverter(const APIConverter&) = delete;
    APIConverter& operator=(const APIConverter&) = delete;

    // --- Configuration Methods ---

    // Sets the API key required for requests.
    void setApiKey(const std::string& key);
    // Sets the location for which to fetch weather data.
    void setLocation(const std::string& loc);
    // Sets the desired units ("Metric" or "Imperial"). Returns false if invalid.
    bool setUnits(const std::string& unit);

    // --- API Interaction Methods ---

    // Fetches current weather data from the API.
    // Returns a unique_ptr to a CurrentWeatherReport, or nullptr on failure.
    std::unique_ptr<CurrentWeatherReport> getCurrentWeather();

    // Fetches forecast data (daily/hourly) from the API for a specified number of days.
    // Returns a unique_ptr to a ForecastReport, or nullptr on failure.
    std::unique_ptr<ForecastReport> getForecastReport(int days, ForecastReport::DetailLevel detail);
};

#endif // APICONVERTER_H