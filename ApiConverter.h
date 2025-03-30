//
// Created by Majdp on 27-Mar-2025.
//
#ifndef APICONVERTER_H
#define APICONVERTER_H

#include <string>
#include "Weather.h"
#include "nlohmann/json.hpp"
#include "httplib.h"

class APIConverter {
  private:
  httplib::Client client;
  std::string apiKey;
  std::string location;

  public:
  APIConverter();
  APIConverter(const std::string& key, const std::string& loc);

  // Accessors
  std::string getApiKey() const;
  std::string getLocation() const;

  // Mutators
  void setApiKey(const std::string& key);
  void setLocation(const std::string& loc);

  // API Interaction
  void updateWeather(Weather* weather);
};

#endif // APICONVERTER_H
