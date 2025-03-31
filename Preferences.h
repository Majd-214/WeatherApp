//
// Created by Ryan C on 3/29/2025.
//

#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <string>
using namespace std;

class Preferences {
public:
  string location;
  string units;
  string datamode;
  string forecast;

  Preferences();
  Preferences(const string &location, const string &units,
              const string &datamode, const string &forecast);

  bool fileloading(const string &filename);
  bool filesaving(const string &filename) const;
};

#endif // PREFERENCES_H