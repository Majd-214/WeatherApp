//
// Created by aburasm on 2025-03-19.
//

#ifndef PROPERTIES_H
#define PROPERTIES_H

#include <iostream>
#include <string>

using namespace std;

class Properties {
 protected:
  string unit;
  double value;

 public:
  Properties();

  double getValue() const;
  string getUnit() const;

  void setValue(double value);
  void setUnit(string unit);
};

#endif  // PROPERTIES_H
