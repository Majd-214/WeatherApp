//
// Created by aburasm on 2025-03-19.
//

#include "Property.h"

Property::Property() {
  this->name = "N/A";
  this->value = 0;
  this->unit = "N/A";
}

Property::Property(string name, double value, string unit) {
  this->name = name;
  this->value = value;
  this->unit = unit;
}

string Property::getName() const { return this->name; }
double Property::getValue() const { return this->value; }
string Property::getUnit() const { return this->unit; }
void Property::setName(string name) { this->name = name; }
void Property::setValue(double value) { this->value = value; }
void Property::setUnit(string unit) { this->unit = unit; }

ostream &operator<<(ostream &os, const Property &p) {
  os << "Property: " << p.getName() << " | Value: " << p.getValue() << " | Unit: " << p.unit << endl;
  return os;
}
