// APIConverter.h
#ifndef APICONVERTER_H
#define APICONVERTER_H

#include <string>
#include "Weather.h"
#include "Forecast.h"
// Removed redundant includes already in other headers if not directly used here
#include "httplib.h" // Keep httplib as it's used for the client member

// Forward declare json if only pointers/references are used in header
// #include "nlohmann/json_fwd.hpp"
// Or include the full header if needed
#include "nlohmann/json.hpp"


class APIConverter {
private:
    httplib::Client client;
    std::string apiKey;
    std::string location;
    std::string units;

public:
    APIConverter();
    APIConverter(const std::string& key, const std::string& loc);

    // Accessors
    std::string getApiKey() const;
    std::string getLocation() const;
    std::string getUnits() const; // <<< Added missing declaration here
    void setUnits(const std::string& unit);


    // Mutators
    void setApiKey(const std::string& key);
    void setLocation(const std::string& loc);

    // API Interaction
    void updateWeather(Weather* weather);
    Forecast* getForecast(int days);
};

#endif // APICONVERTER_H