#include "robot.hpp"

using namespace std;

Robot::Robot() : lidar(vector<uint16_t>(271, 0))
{}

Robot::~Robot() {}

void Robot::moveForward() 
{
    cout << "[robot] Move forward" << endl;
}

void Robot::moveBackward() 
{
    cout << "[robot] Move backward" << endl;
}

void Robot::turnLeft() 
{
    cout << "[robot] Turn left" << endl;
}

void Robot::turnRight() 
{
    cout << "[robot] Turn right" << endl;
}

Accelerometer& Robot::getAccelerometer()
{
    return accelerometer;
}

void Robot::setAccelerometer(const Accelerometer& newAcc) 
{
    accelerometer = newAcc;
}

Gyroscope& Robot::getGyroscope() 
{
    return gyroscope;
}

void Robot::setGyroscope(const Gyroscope& newGyro) 
{
    gyroscope = newGyro;
}

Gps& Robot::getGps() 
{
    return gps;
}

void Robot::setGps(const Gps& newGps) 
{
    gps = newGps;
}

vector<uint16_t>& Robot::getLidar() 
{
    return lidar;
}

void Robot::setLidar(vector<uint16_t>& newLidar) 
{
    lidar = newLidar;
}
