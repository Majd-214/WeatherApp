//
// Created by Ryan C on 3/29/2025.
//

#include "Preferences.h"
#include <fstream>
#include <iostream>
#include <sstream>

Preferences::Preferences()
    : location("Unknown"), units("Metric"), datamode("advanced") {}

Preferences::Preferences(const string &location, const string &units,
                         const string &datamode)
    : location(location), units(units), datamode(datamode) {}

bool Preferences::fileloading(const string &filename) {
  ifstream infile(filename);

  if (!infile.is_open()) {
    return false;
  }

  string line;
  while (getline(infile, line)) {
    istringstream linestream(line);
    string key;

    if (getline(linestream, key, ':')) {
      string value;

      if (getline(linestream, value)) {
        while (!value.empty() && (value[0] == ' ' || value[0] == '\n')) {
          value.erase(0, 1);
        }

        if (key == "location") {
          location = value;
        }

        else if (key == "units") {
          units = value;
        }

        else if (key == "datamode") {
          datamode = value;
        }
      }
    }
  }
  infile.close();
  return true;
}

bool Preferences::filesaving(const string &filename) const {
  ofstream outfile(filename);

  if (!outfile.is_open()) {
    cout << "Error opening file." << endl;
    return false;
  }

  outfile << "Location: " << location << endl;
  outfile << "Units: " << units << endl;
  outfile << "Datamode: " << datamode << endl;
  outfile.close();
  return true;
}