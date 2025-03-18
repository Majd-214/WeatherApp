# Console Based Weather Application

## Overview

This project is a console-based weather application developed in C++ as part of an object-oriented programming assignment. The application aims to provide users with current weather information and a simple forecast for a specified location. It demonstrates various C++ language features and object-oriented programming principles.

## Features

* **Current Weather Display:** Shows the current temperature, humidity, UV index, and potentially other weather properties.
* **Simple Forecast:** Provides a basic forecast, including temperature and conditions.
* **User Location Input:** Reads the user's location from a file.
* **User Settings:** Reads user preferences (like temperature units) from a file.
* **Data Retrieval:** Fetches weather data from an external API (implementation details would be in the source code).
* **Object-Oriented Design:** Utilizes classes, inheritance, polymorphism, and encapsulation.

## Class Structure

The application is structured using the following classes and structs:

* **`Weather` Class:**
    * Manages an array of `Properties`.
    * Holds a pointer to a `Forecast`.
    * Responsible for storing overall weather information.
* **Abstract Class `Properties`:**
    * Base class for different weather properties.
    * Defines virtual methods for getting the value, unit, and predicting the property.
* **Derived Classes (inheriting from `Properties`):**
    * `Temperature`: Stores temperature value and unit (Celsius or Fahrenheit).
    * `Humidity`: Stores humidity value (in percentage).
    * `UV`: Stores UV index value.
* **`Wind` Struct:**
    * Holds wind speed, unit (km/h or mph), and direction.
* **`Forecast` Struct:**
    * Stores forecast temperature and conditions.
* **`APIConverter` Class:**
    * Responsible for fetching weather data from an API and converting it into a `Weather` object.
* **`UserInterface` Class:**
    * Handles user interaction through the console.
    * Displays weather information.
    * Reads user location and settings from files.
* **`WeatherPrediction` Class:**
    * Provides a basic weather forecast based on the current weather data, utilizing polymorphism through the `predict()` method in the `Properties` hierarchy.

## OOP Concepts Used

* **Classes:** The core building blocks of the application, used to model weather data, API interaction, user interface, and prediction logic.
* **Abstract Class:** The `Properties` class serves as an abstract base class, defining a common interface for all weather-related properties.
* **Inheritance:** The `Temperature`, `Humidity`, and `UV` classes inherit from the `Properties` class, demonstrating specialization and code reuse.
* **Polymorphism:** The `WeatherPrediction` class utilizes the virtual `predict()` method in the `Properties` hierarchy to perform different prediction logic based on the specific type of weather property.
* **Encapsulation:** Each class manages its own data and provides controlled access through public methods, ensuring data integrity.
* **Aggregation:** The `Weather` class aggregates multiple `Properties` objects.

## File I/O

The application utilizes file input/output for the following:

* **User Location:** The `UserInterface` reads the user's location from a file.
* **User Settings:** The `UserInterface` reads user preferences, such as the preferred temperature unit, from a file.

## How to Build and Run

Instructions on how to compile and run the application will be provided here (assuming a standard C++ development environment):

1.  **Prerequisites:** Ensure you have a C++ compiler (like g++) installed on your system.

2.  **Compilation:** Navigate to the project directory in your terminal and use the compiler to compile the source code files (e.g., `g++ main.cpp weather.cpp api_converter.cpp user_interface.cpp weather_prediction.cpp -o weatherapp`).

3.  **Running:** Execute the compiled application (e.g., `./weatherapp`).

4.  **Input Files:** Make sure you have the necessary input files (for location and settings) in the same directory as the executable or provide the correct file paths.

## Deliverables

This project fulfills the requirements for the C++ project assignment, including:

* A project proposal (submitted separately).
* Well-commented source code.
* A video recording of the project presentation.
* A project report (as per the provided template).
* A zip folder containing the project's source code.
* A project presentation.

## Further Improvements

Potential future enhancements could include:

* More detailed weather information (e.g., wind speed, pressure).
* Hourly and daily forecasts.
* Error handling for API calls and file operations.
* More sophisticated prediction algorithms.
* A more user-friendly console interface.

## Group Members

* Majd Aburas - 400509985
* Joshua Kam - 400572866
* Ryan Chung - 400577912
* Vipra Checkera 400594097

## Project Title

Console Based Weather Application

## Problem Description

This console application provides users with current weather information and a simple forecast for a given location. It aims to demonstrate object-oriented programming principles in C++ and fulfill the requirements of the course project.

## C++ Concepts Used

* Pointers
* Concrete and abstract classes
* Overloaded functions and constructors (implementation details in source code)
* Inheritance
* Polymorphism
* File I/O
* Dynamic arrays (using pointers)

## Key Functions and Classes Description

*(Refer to the descriptions provided earlier for `predict`, `convertToWeather`, and `displayWeather`)*

This README provides a comprehensive overview of the Console Based Weather Application project. For detailed implementation, please refer to the well-commented source code.
