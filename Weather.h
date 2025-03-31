// Weather.h
#ifndef WEATHER_H
#define WEATHER_H

#include "Property.h"

enum PropertyIndex {
  TEMPERATURE = 0,
  FEELS_LIKE = 1,
  WIND_SPEED = 2,
  WIND_DIRECTION = 3,
  HUMIDITY = 4,
  PRESSURE = 5,
  VISIBILITY = 6,
  UV = 7,
  GUST_SPEED = 8,
  PRECIPITATION = 9,
  CLOUD = 10,
  LAST_UPDATED = 11,
  NUM_PROPERTIES = 12
};

class Weather {
  private:
  Property* properties[NUM_PROPERTIES];

  public:
  Weather();
  ~Weather();
  void setProperty(PropertyIndex index, Property* property);
  Property* getProperty(PropertyIndex index) const;
};

#endif // WEATHER_H