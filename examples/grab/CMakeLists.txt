cmake_minimum_required(VERSION 3.10)
project(pylon)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread")
find_package(Pylon REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

find_package(OpenCV REQUIRED)

include_directories(${Pylon_INCLUDE_DIRS})
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/src
)

file(GLOB allSources
        "${PROJECT_SOURCE_DIR}/include/*.h"
        "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

add_executable(grabCV src/grabCV.cpp)
target_link_libraries(grabCV ${Pylon_LIBRARIES} ${OpenCV_LIBRARIES})

add_executable(grabEvery src/grabEvery.cpp)
target_link_libraries(grabEvery ${Pylon_LIBRARIES} ${OpenCV_LIBRARIES})