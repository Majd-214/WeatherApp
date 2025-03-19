//
// Created by aburasm on 2025-03-19.
//

#ifndef PROPERTIES_H
#define PROPERTIES_H

#include <iostream>
#include <string>

using namespace std;

class Properties {
 private:
  string name, unit;
  double value;

 public:
  Properties();
  Properties(string name, double value, string unit);

  string getName() const;
  double getValue() const;
  string getUnit() const;

  void setValue(double value);
  void setName(string name);
  void setUnit(string unit);
};

#endif  // PROPERTIES_H
