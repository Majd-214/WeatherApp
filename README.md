# Console Weather Application

## Overview

This C++ console application fetches and displays current weather conditions and multi-day forecasts using the WeatherAPI.com service. It demonstrates object-oriented principles and utilizes CMake for cross-platform building.

## Features

* **Current Weather:** Displays detailed current conditions (temperature, feels like, wind direction/speed, humidity, pressure, visibility, precipitation, etc.). Wind direction shows both cardinal (e.g., NW) and degrees.
* **Forecast:** Provides daily summary forecasts and detailed hourly forecasts for a configurable number of days (1-3). Includes the day of the week (e.g., Tuesday). Daily summaries show wind direction with cardinal and degrees; hourly tables show cardinal only for brevity.
* **Configurable Settings:**
    * API Key management (prompts user if missing).
    * Location setting (accepts city name, zip code, lat/lon).
    * Unit selection (Metric/Imperial).
    * Number of forecast days.
* **Persistence:** Saves and loads settings (API Key, Location, Units, Forecast Days) to/from a `settings.txt` file in the same directory as the executable.
* **User-Friendly Interface:** Simple console menu for navigation and interaction.
* **Build System:** Uses CMake for standardized, cross-platform building.
* **External Libraries:** Includes `httplib` for HTTP requests and `nlohmann/json` for parsing API responses.

## Prerequisites

Before you can build and run this project, you need the following installed on your system:

1.  **CMake:** A cross-platform build system generator.
    * **What it is:** CMake reads the `CMakeLists.txt` file in the project and generates the actual build files (like Makefiles or Visual Studio solutions) suitable for your system. It makes building easier on different computers.
    * **How to get it:** Download from the [official CMake website](https://cmake.org/download/). Follow the installation instructions for your operating system. Make sure the `cmake` command is accessible from your terminal or command prompt after installation (you might need to check an option like "Add CMake to the system PATH" during installation).
2.  **C++ Compiler:** A compiler that supports C++14 or later.
    * **Examples:** g++ (usually default on Linux, available on macOS via Xcode Command Line Tools, on Windows via MinGW/MSYS2/WSL), Clang (available on major OS), Microsoft Visual C++ (installed with Visual Studio on Windows).
    * **How to check:** Open your terminal/command prompt and try typing `g++ --version`, `clang++ --version`, or check if Visual Studio with C++ tools is installed.
3.  **OpenSSL Development Libraries (Recommended):** Needed by the `httplib` library for potentially handling HTTPS communication securely. While the current API URL might be HTTP, including this makes the application more robust.
    * **Linux (Debian/Ubuntu):** Open a terminal and run: `sudo apt-get update && sudo apt-get install libssl-dev`
    * **Linux (Fedora/CentOS/RHEL):** Open a terminal and run: `sudo dnf install openssl-devel` (or `sudo yum install openssl-devel`)
    * **macOS (using Homebrew):** Open Terminal and run: `brew update && brew install openssl`. Homebrew might give instructions on how CMake can find it, but often it works automatically.
    * **Windows:** This is often the trickiest. A common source is [Shining Light Productions](https://slproweb.com/products/Win32OpenSSL.html). Download an installer (choose a recent version, **not** the "Light" one, ensure it matches your system 32-bit/64-bit). During installation, choose to copy the DLLs to the OpenSSL binaries directory or system directory. You might need to tell CMake where you installed it (see Troubleshooting below).
4.  **WeatherAPI.com API Key:** A free API key is required to fetch weather data.
    * **How to get it:** Sign up at [WeatherAPI.com](https://www.weatherapi.com/). Keep your key safe!

*(If `httplib.h` or `nlohmann/` are inside another subdirectory, like `include/`, the `CMakeLists.txt` file might need a slight adjustment to the `target_include_directories` command.)*
## Building with CMake (Step-by-Step)

CMake makes building easy across different platforms. Here are the common ways:

**Option 1: Command Line (Recommended for Simplicity & Cross-Platform)**

This method works on Linux, macOS, and Windows (using Command Prompt, PowerShell, Git Bash, etc., provided CMake and a compiler are in your system's PATH).

1.  **Open Terminal/Command Prompt:** Use your system's terminal application. Navigate (`cd`) to the root directory of the project (e.g., `Your_Project_Folder/` which contains `CMakeLists.txt`).
2.  **Create Build Directory:** It's standard practice to keep build files separate from source files. Create a directory named `build` and move into it.
    ```bash
    mkdir build
    cd build
    ```
3.  **Configure with CMake:** Run the `cmake` command, telling it where the source code (and `CMakeLists.txt`) is located. The `..` means "the parent directory". CMake will detect your compiler, check for dependencies like OpenSSL, and generate the native build files (e.g., Makefiles).
    ```bash
    cmake ..
    ```
    * **Troubleshooting (Windows - OpenSSL):** If `cmake ..` fails because it can't find OpenSSL, and you installed it to a custom location (e.g., `C:\OpenSSL-Win64`), you might need to tell CMake where it is:
        ```bash
        cmake .. -DOPENSSL_ROOT_DIR="C:/OpenSSL-Win64"
        ```
      (Adjust the path to your actual OpenSSL installation directory).
    * **Troubleshooting (Other):** Read any error messages carefully. They often indicate missing prerequisites or configuration issues.
4.  **Build the Project:** Once CMake has configured successfully, run the build command. This tells CMake to use the generated native build tools (like Make or MSBuild) to compile your code.
    ```bash
    cmake --build .
    ```
    * *Alternative (Linux/macOS):* If CMake generated Makefiles (which is common), you can usually just run `make` inside the `build` directory after running `cmake ..`.
5.  **Result:** If everything works, the compiler will process your source files. Upon completion, you will find the executable file (e.g., `WeatherApp.exe` on Windows, `WeatherApp` on Linux/macOS) inside the `build` directory you created.

**Option 2: Using an IDE (CLion, Visual Studio, VS Code)**

Modern IDEs often simplify the CMake process.

* **CLion:**
    1.  Go to `File` > `Open`.
    2.  Select the project's root folder (the one containing `CMakeLists.txt`).
    3.  CLion should automatically detect `CMakeLists.txt`, load the project, and configure CMake.
    4.  You can then use the Build button (hammer icon) or Run/Debug buttons to compile and execute.
* **Visual Studio (2017 and later):**
    1.  Go to `File` > `Open` > `Folder...`.
    2.  Select the project's root folder.
    3.  Visual Studio detects `CMakeLists.txt` and starts configuring. Watch the Output window (CMake tab) for progress or errors.
    4.  Once configured, select the desired build type (e.g., Debug/Release) and target (`WeatherApp.exe`) from the dropdowns in the toolbar.
    5.  Use `Build` > `Build Solution` (or `Build All`) to compile.
* **VS Code:**
    1.  Install the "CMake Tools" extension from the VS Code Marketplace.
    2.  Go to `File` > `Open Folder...` and select the project's root folder.
    3.  The CMake Tools extension should activate. It might ask you to select a "Kit" (compiler set) the first time.
    4.  Use the commands in the status bar (bottom of the window) or the Command Palette (`Ctrl+Shift+P`) to `CMake: Configure` and `CMake: Build`.

## Running the Application

1.  **Locate Executable:** The executable file (`WeatherApp.exe` or `WeatherApp`) will be inside your `build` directory after a successful build.
2.  **Navigate:** Open your system's terminal or command prompt. Navigate **into the `build` directory**.
    ```bash
    cd path/to/Your_Project_Folder/build
    ```
3.  **API Key / Settings:**
    * The application looks for `settings.txt` **in the directory where you run the executable from** (which should be the `build` directory).
    * **First Run:** If `settings.txt` doesn't exist, the app will prompt you: "Please enter your WeatherAPI key: ". Paste your key and press Enter. The app will then create `settings.txt` for you in the `build` directory.
    * **Manual Setup:** You can create `settings.txt` in the `build` directory yourself *before* running. At minimum, it needs your API key:
        ```
        apikey:YOUR_ACTUAL_API_KEY
        ```
      You can optionally add other settings:
        ```
        location:Hamilton
        units:Metric
        forecastdays:3
        ```
4.  **Run:** Execute the application from the terminal while you are *inside* the `build` directory:
    * **Windows:** `.\WeatherApp.exe`
    * **Linux/macOS:** `./WeatherApp`
5.  **Interact:** Use the menu options displayed in the console.

## Core Class Structure

* **`main.cpp`**: Entry point, main application loop, orchestrates UI, Preferences, and API calls.
* **`UI` (Static Class)**: Handles all console input and output, including menus, prompts, and report display.
* **`Preferences`**: Manages loading, saving, and accessing user settings (API key, location, units, etc.) from `settings.txt`.
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