//
// Created by Ryan C on 3/19/2025.
//

#include "Humidity.h"
Humidity::Humidity() : Properties(0, "%") {}
Humidity::Humidity(Properties &property) {
  this->unit = property.getUnit();
  this->value = property.getValue();
}
Humidity::Humidity(double value, string unit) : Properties(value, unit) {}