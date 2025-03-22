//
// Created by Joshua Kam on 2025-03-19.
//

#include "Temperature.h"
Temperature::Temperature() : Properties(0, "%") {}
Temperature::Temperature(Properties &property) {
  this->unit = property.getUnit();
  this->value = property.getValue();
}