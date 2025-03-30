#include "UI.h"
#include <iostream>

using namespace std;

void displayWeather(const Weather& weather) {
    cout << "\nCurrent Weather Conditions:" << endl; 
    for (int i = 0; i < NUM_PROPERTIES; ++i) {
        Property* prop = weather.getProperty(static_cast<PropertyIndex>(i));
        if (prop != nullptr) {
            cout << *prop;
        } else {
            cout << "Property " << i << " not set." << endl;
        }
    }
}

void updateWeather(Weather& weather) {
    int choice;
    cout << "\nChoose a property to update:" << endl;
    cout << "0 - Temperature\n1 - Humidity\n2 - Wind Speed\nEnter choice: ";
    cin >> choice;

    if (choice < 0 || choice >= NUM_PROPERTIES) {
        cout << "Invalid choice!" << endl;
        return;
    }

    Property* prop = weather.getProperty(static_cast<PropertyIndex>(choice));

    if (prop != nullptr) {
        if (choice == 0)
            cout << "Property: " << prop->getName() << endl;
        if (choice == 1)
            cout << " | Value: " << prop->getValue() << endl;
        if (choice == 2)
            cout << " " << prop->getUnit() << endl;
    } else {
        cout << "Property not set!" << endl;
    }

    string name;
    double value;
    string unit;

    cout << "Enter property name: ";
    cin >> name;
    cout << "Enter value: ";
    cin >> value;
    cout << "Enter unit: ";
    cin >> unit;

    Property* newProp = new Property(name, value, unit);
    weather.setProperty(static_cast<PropertyIndex>(choice), newProp);
    cout << "Property updated successfully!" << endl;
}



int main() {
    Weather weather;
    int option;

    do {
        cout << "\nWeather App Menu:" << endl;
        cout << "1. Display Weather" << endl;
        cout << "2. Update Weather Property" << endl;
        cout << "3. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> option;

        switch (option) {
        case 1:
            displayWeather(weather);
            break;
        case 2:
            updateWeather(weather);
            break;
        case 3:
            cout << "Exiting..." << endl;
            break;
        default:
            cout << "Invalid option! Try again." << endl;
        }
    } while (option != 3);

    return 0;
}