// Weather.h
#ifndef WEATHER_H
#define WEATHER_H

#include "Property.h"

enum PropertyIndex {
  TEMPERATURE = 0,
  FEELS_LIKE = 1,
  HIGH_TEMP = 2,
  LOW_TEMP = 3,
  AIR_QUALITY = 4,
  UV = 5,
  WIND_SPEED = 6,
  GUST = 7,
  WIND_DIRECTION = 8,
  SUNRISE = 9,
  SUNSET = 10,
  PRECIPITATION = 11,
  VISIBILITY = 12,
  HUMIDITY = 13,
  PRESSURE = 14,
  NUM_PROPERTIES = 15 // UPDATE to Latest Index
};

class Weather {
private:
  Property *properties[NUM_PROPERTIES];

public:
  Weather();
  ~Weather();
  void setProperty(PropertyIndex index, Property *property);
  Property *getProperty(PropertyIndex index) const;
};




#endif // WEATHER_H