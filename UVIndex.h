#ifndef UVINDEX_H
#define UVINDEX_H
#include "properties.h"

#include <iostream>
#include <string>

using namespace std;

class UVIndex : protected Properties {
 public:
  string level;  // low, moderate, high, very high, extreme
  int index;     // 0-10
  int peak;      // 0-10
  int hour;      // 1-12


  //parametized (base) constructor

  //accessor

  //getter


};

#endif  // UVINDEX_H


// Forecast* predict override