//
// Created by aburasm on 2025-03-19.
//

#include "Property.h"
Property::Property() {
  this->value = 0;
  this->unit = "N/A";
}
Property::Property(double value, string unit) {
  this->value = value;
  this->unit = unit;
}
double Property::getValue() const { return this->value; }
string Property::getUnit() const { return this->unit; }
void Property::setValue(double value) { this->value = value; }
void Property::setUnit(string unit) { this->unit = unit; }