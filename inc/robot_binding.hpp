#ifndef ROBOT_BINDING_HPP
#define ROBOT_BINDING_HPP

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "robot.hpp"

namespace py = pybind11;

PYBIND11_EMBEDDED_MODULE(naio, m) {
    py::class_<Robot, std::shared_ptr<Robot>>(m, "Robot")
        .def(py::init<>())
        .def("moveForward", &Robot::moveForward)
        .def("moveBackward", &Robot::moveBackward)
        .def("turnLeft", &Robot::turnLeft)
        .def("turnRight", &Robot::turnRight)
        .def_property("accelerometer", &Robot::getAccelerometer, &Robot::setAccelerometer)
        .def_property("gyroscope", &Robot::getGyroscope, &Robot::setGyroscope)
        .def_property("gps", &Robot::getGps, &Robot::setGps)
        .def_property("lidar", &Robot::getLidar, &Robot::setLidar)
        ;

    py::class_<Accelerometer>(m, "Accelerometer")
        .def(py::init<int16_t, int16_t, int16_t>(), 
            py::arg("xVal") = 0, py::arg("yVal") = 0, py::arg("zVal") = 0)
        .def_readwrite("x", &Accelerometer::x)
        .def_readwrite("y", &Accelerometer::y)
        .def_readwrite("z", &Accelerometer::z)
        ;

    py::class_<Gyroscope>(m, "Gyroscope")
        .def(py::init<int16_t, int16_t, int16_t>(),
            py::arg("xVal") = 0, py::arg("yVal") = 0, py::arg("zVal") = 0)
        .def_readwrite("x", &Gyroscope::x) 
        .def_readwrite("y", &Gyroscope::y) 
        .def_readwrite("z", &Gyroscope::z) 
        ;

    py::class_<Gps>(m, "Gps")
        .def(py::init<double, double, double, double, uint8_t, uint8_t>(),
            py::arg("timeVal") = 0, py::arg("latVal") = 0, py::arg("lonVal") = 0, 
            py::arg("altVal") = 0, py::arg("unitVal") = 0, py::arg("numberOfSatVal") = 0)
        .def_readwrite("time", &Gps::time) 
        .def_readwrite("lat", &Gps::lat) 
        .def_readwrite("lon", &Gps::lon) 
        .def_readwrite("alt", &Gps::alt) 
        .def_readwrite("unit", &Gps::unit) 
        .def_readwrite("numberOfSat", &Gps::numberOfSat) 
        ;
}

#endif
