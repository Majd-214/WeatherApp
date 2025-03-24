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
 string unit, name;

public:
 Property();

 Property(string name, double value, string unit);

 string getName() const;

 double getValue() const;

 string getUnit() const;

 void setName(string name);

 void setValue(double value);

 void setUnit(string unit);

 friend ostream &operator<<(ostream &os, const Property &property);
};

#endif  // PROPERTY_H
