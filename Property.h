// Property.h
#ifndef PROPERTY_H
#define PROPERTY_H

#include <iostream> // For friend ostream operator
#include <string>   // For name and unit strings

// Represents a single weather property (like temperature, humidity) with its value and unit.
class Property {
 // Use private for encapsulation, grant access via public methods.
 private:
 std::string name;  // Descriptive name of the property (e.g., "Temperature").
 double value;      // Numerical value of the property.
 std::string unit;  // Unit of measurement (e.g., "°C", "%", "km/h").

 public:
 // Default constructor: Initializes with placeholder values.
 Property();

 // Parameterized constructor: Initializes with specific name, value, and unit.
 Property(const std::string& name, double value, const std::string& unit);

 // --- Getters (Provide read-only access) ---
 std::string getName() const;
 double getValue() const;
 std::string getUnit() const;

 // --- Setters (Allow modification) ---
 void setName(const std::string& name);
 void setValue(double value);
 void setUnit(const std::string& unit);

 // Friend function to allow direct printing of Property objects using std::cout.
 friend std::ostream& operator<<(std::ostream& os, const Property& prop);
};

#endif // PROPERTY_H