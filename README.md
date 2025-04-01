# Console Weather Application

## Overview

This C++ console application fetches and displays current weather conditions and multi-day forecasts using the WeatherAPI.com service. It demonstrates object-oriented principles, including abstraction, inheritance, polymorphism, and resource management (RAII with `std::unique_ptr`).

## Features

* **Current Weather:** Displays detailed current conditions (temperature, feels like, wind, humidity, pressure, visibility, precipitation, etc.).
* **Forecast:** Provides daily summary forecasts and detailed hourly forecasts for a configurable number of days (1-14).
* **Configurable Settings:**
    * API Key management (prompts user if missing).
    * Location setting (accepts city name, zip code, lat/lon).
    * Unit selection (Metric/Imperial).
    * Number of forecast days.
* **Persistence:** Saves and loads settings (API Key, Location, Units, Forecast Days) to/from a `settings.txt` file.
* **User-Friendly Interface:** Simple console menu for navigation and interaction.
* **External Libraries:** Uses `httplib` for HTTP requests and `nlohmann/json` for parsing API responses. (These libraries need to be available during compilation/linking).

## Core Class Structure

* **`main.cpp`**: Entry point, main application loop, orchestrates UI, Preferences, and API calls.
* **`UI` (Static Class)**: Handles all console input and output, including menus, prompts, and report display.
* **`Preferences`**: Manages loading, saving, and accessing user settings (location, units, API key, etc.) from `settings.txt`.
* **`APIConverter`**: Interfaces with the WeatherAPI. Constructs requests, performs HTTP calls (using `httplib`), parses JSON responses (using `nlohmann/json`), and converts data into `Weather` and `Forecast` objects. Creates report objects.
* **`Weather`**: Container class holding various weather `Property` objects for a specific time or summary period. Manages `Property` object lifetimes.
* **`Property`**: Represents a single weather data point (e.g., Temperature) with its name, value, and unit.
* **`Forecast`**: Container holding `DailyForecast` objects.
* **`DailyForecast`**: Represents one day's forecast, containing a summary `Weather` object and a vector of `HourlyForecast` objects.
* **`HourlyForecast`**: Represents one hour's forecast, containing a `Weather` object and a time string.
* **`IDisplayable` (Interface)**: Abstract base class defining the `display(ostream&)` contract.
* **`WeatherReport` (Abstract Class)**: Abstract base for reports, inheriting `IDisplayable` and adding `getReportType()`.
* **`CurrentWeatherReport`**: Concrete report class holding `Weather` data for current conditions. Implements `display`.
* **`ForecastReport`**: Concrete report class holding `Forecast` data. Implements `display` to show either daily or hourly details based on configuration.

## Key OOP Concepts Demonstrated

* **Abstraction:** `IDisplayable` and `WeatherReport` define interfaces and abstract concepts.
* **Encapsulation:** Classes manage their own data (`Preferences`, `Weather`, `Property`). Internal details of API calls are hidden within `APIConverter`.
* **Inheritance:** `WeatherReport` inherits from `IDisplayable`. `CurrentWeatherReport` and `ForecastReport` inherit from `WeatherReport`.
* **Polymorphism:** The `UI::displayReport` function uses an `IDisplayable&` reference, allowing it to display any concrete `WeatherReport` type through virtual function calls (`display`).
* **Composition/Aggregation:** `Weather` holds `Property` objects. `Forecast` holds `DailyForecast` objects, which hold `HourlyForecast` and `Weather` objects. `APIConverter` uses an `httplib::Client`.
* **RAII (Resource Acquisition Is Initialization):** `std::unique_ptr` is used in `APIConverter` to manage the `httplib::Client` lifetime. `Weather` manages the lifetime of `Property` pointers through constructors/destructor/copy-assignment (Rule of Three). Report objects are managed by `std::unique_ptr` in `main`.

## Dependencies

* A C++11 compliant compiler (or later).
* **cpp-httplib:** A header-only cross-platform HTTP/HTTPS library. You need to download `httplib.h` and place it where your compiler can find it (e.g., in the project directory or an include path). Get it from [https://github.com/yhirose/cpp-httplib](https://github.com/yhirose/cpp-httplib).
* **JSON for Modern C++:** A header-only JSON library by Niels Lohmann. You need `json.hpp`. It's often placed within a `nlohmann` directory. Get it from [https://github.com/nlohmann/json](https://github.com/nlohmann/json).

## How to Build and Run

1.  **Get Dependencies:** Download `httplib.h` and `nlohmann/json.hpp`.
2.  **Arrange Files:** Place `httplib.h` and the `nlohmann` directory (containing `json.hpp`) alongside your source files (`.cpp`, `.h`) or in a location specified by your compiler's include path.
3.  **Compile:** Use a C++ compiler (like g++ or clang++). You need to link necessary libraries if `httplib` requires them (e.g., OpenSSL for HTTPS, pthreads). A simple compilation command might look like:
    ```bash
    g++ main.cpp UI.cpp Preferences.cpp APIConverter.cpp Weather.cpp Property.cpp CurrentWeatherReport.cpp ForecastReport.cpp WeatherReport.cpp -o weather_app -std=c++11 -pthread -lssl -lcrypto
    ```
    *(Note: The exact libraries `-lssl -lcrypto` might vary based on your system and how `httplib` was configured/built if not purely header-only for HTTPS).*
4.  **API Key:** Run the application. If `settings.txt` doesn't exist or lacks an API key, you will be prompted to enter one. Get a free API key from [WeatherAPI.com](https://www.weatherapi.com/).
5.  **Run:** Execute the compiled application:
    ```bash
    ./weather_app
    ```
6.  **Settings File (`settings.txt`):** The application will create/use a `settings.txt` file in the same directory to store your preferences:
    ```
    apikey:YOUR_API_KEY_HERE
    location:London
    units:Metric
    datamode:advanced
    forecastdays:3
    ```

## Further Improvements

* Implement Move Semantics (Rule of Five) for `Weather`, `DailyForecast`, `HourlyForecast` for potentially better performance.
* More robust error handling for API responses and network issues.
* Add input validation for location strings.
* Refine the display formatting.
* Integrate unit testing.
* Consider using a build system like CMake to manage dependencies and compilation.