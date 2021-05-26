cmake_minimum_required(VERSION 3.10)
project(COVID-19)

set(CMAKE_CXX_STANDARD 11)

configure_file(images/map.jpg images/map.jpg COPYONLY)
configure_file(images/icon.png images/icon.png COPYONLY)
configure_file(fonts/ObliviousFont.ttf fonts/ObliviousFont.ttf COPYONLY)
configure_file(data/confirmed_cases.txt data/confirmed_cases.txt COPYONLY)

# Define sources and executable
set(EXECUTABLE_NAME "COVID-19")

add_executable(${EXECUTABLE_NAME} main.cpp MyList.cpp MyList.h Node.cpp Node.h Node.cpp Node.h MyList.cpp MyList.h)

# Detect and add SFML
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
#Find any version 2.X of SFML
#See the FindSFML.cmake file for additional details and instructions
find_package(SFML 2 REQUIRED network audio graphics window system)
if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
endif()