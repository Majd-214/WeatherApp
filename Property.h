//
// Created by aburasm on 2025-03-19.
//

#ifndef PROPERTY_H
#define PROPERTY_H

#include <iostream>
#include <string>

using namespace std;

class Property {
 protected:
  double value;
  string unit;

 public:
  Property();
  Property(double value, string unit);

  double getValue() const;
  string getUnit() const;

  void setValue(double value);
  void setUnit(string unit);
};

#endif  // PROPERTY_H
