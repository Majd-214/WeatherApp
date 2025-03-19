//
// Created by aburasm on 2025-03-19.
//

#include "Properties.h"
Properties::Properties() {
  this->value = 0;
  this->unit = "N/A";
}
Properties::Properties(double value, string unit) {
  this->value = value;
  this->unit = unit;
}
double Properties::getValue() const { return this->value; }
string Properties::getUnit() const { return this->unit; }
void Properties::setValue(double value) { this->value = value; }
void Properties::setUnit(string unit) { this->unit = unit; }