cmake_minimum_required(VERSION 3.27)
project(BT)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find libcurl
find_package(CURL REQUIRED)

# Find the Qt packages
find_package(Qt6 COMPONENTS Core Widgets REQUIRED)

# Include libcurl directories
include_directories(${CURL_INCLUDE_DIRS})

# Add the Qt5 modules
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Add Qt5 definitions
add_definitions(${QT_DEFINITIONS})

# Add the executable and include the Qt5 libraries
add_executable(BT main.cpp)

# Link against libcurl libraries
target_link_libraries(BT ${CURL_LIBRARIES} )

# Link against Qt5 libraries
target_link_libraries(BT Qt6::Core Qt6::Widgets)
