//
// Created by Ryan C on 3/19/2025.
//

#ifndef HUMIDITY_H
#define HUMIDITY_H
#include "Properties.h"

class Humidity : protected Properties {
 public:
  Humidity();
  Humidity(Properties &);
  Humidity(double value, string unit);
};

#endif  // HUMIDITY_H
