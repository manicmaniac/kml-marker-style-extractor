project(kml-marker-style-extractor)
cmake_minimum_required(VERSION 3.0)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMakeModules)
include(Findlibkml)
find_package(libkml REQUIRED)
find_package(CURL REQUIRED)
find_package(Boost REQUIRED)

include_directories(
    ${libkml_INCLUDE_DIR}
    ${CURL_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
)

add_definitions(-std=c++11)

add_executable(kml-marker-style-extractor
    main.cc
)
target_link_libraries(kml-marker-style-extractor
    ${libkml_LIBRARIES}
    ${CURL_LIBRARIES}
    ${Boost_LIBRARIES}
)

