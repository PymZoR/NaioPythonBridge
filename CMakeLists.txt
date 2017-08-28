cmake_minimum_required(VERSION 3.0)
project(NaioPythonBridge)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(PythonLibs 2.7 REQUIRED)
add_subdirectory(ext/pybind11)

file(GLOB_RECURSE SOURCES src/*.cpp)

add_executable(NaioPythonBridge ${SOURCES})

include_directories(
    ${NaioPythonBridge_SOURCE_DIR}/inc
    ${PYTHON_INCLUDE_DIRS}
)

target_link_libraries(NaioPythonBridge PRIVATE
    ${PYTHON_LIBRARIES}
    pybind11::embed
)
