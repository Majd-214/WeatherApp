cmake_minimum_required(VERSION 3.11)
project(WeatherApp)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

include(FetchContent)

FetchContent_Declare(
        cpp-httplib
        GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git
        GIT_TAG        v0.14.1 # Or the latest version you want to use
)

FetchContent_MakeAvailable(cpp-httplib)

FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG        v3.11.3 # Or the latest version you want to use
)

FetchContent_MakeAvailable(nlohmann_json)

add_executable(WeatherApp
        main.cpp
        Property.cpp
        Property.h
        Weather.cpp
        Weather.h
)

target_include_directories(WeatherApp PRIVATE ${cpp-httplib_SOURCE_DIR})

target_link_libraries(WeatherApp PRIVATE nlohmann_json)
target_link_libraries(WeatherApp PRIVATE ws2_32)