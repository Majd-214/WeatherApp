// Property.cpp
#include "Property.h"
#include <string>   // Included via header, but good practice
#include <ostream>  // For operator<< implementation

// Default constructor implementation.
Property::Property() : name("N/A"), value(0.0), unit("N/A") {}

// Parameterized constructor implementation. Uses initializer list for efficiency.
Property::Property(const std::string& name, double value, const std::string& unit)
    : name(name), value(value), unit(unit) {}

// --- Getters ---
std::string Property::getName() const { return this->name; }
double Property::getValue() const { return this->value; }
std::string Property::getUnit() const { return this->unit; }

// --- Setters ---
void Property::setName(const std::string& name) { this->name = name; }
void Property::setValue(double value) { this->value = value; }
void Property::setUnit(const std::string& unit) { this->unit = unit; }

// Implementation of the friend overloaded stream insertion operator.
// Defines how a Property object is formatted when sent to an output stream.
std::ostream& operator<<(std::ostream& os, const Property& p) {
  os << "Property Name: " << p.getName()
     << ", Value: " << p.getValue()
     << ", Unit: " << p.getUnit();
  // Avoid adding endl here; let the caller control line breaks.
  return os;
}