#ifndef ROBOT_HPP
#define ROBOT_HPP

#include <iostream>
#include <vector>

using namespace std;

typedef struct Accelerometer {
    Accelerometer(int16_t xVal = 0, int16_t yVal = 0, int16_t zVal = 0) :
        x(xVal), y(yVal), z(zVal) {}
    int16_t x;
    int16_t y;
    int16_t z;
} Accelerometer;

typedef struct Gyroscope {
    Gyroscope(int16_t xVal = 0, int16_t yVal = 0, int16_t zVal = 0) :
        x(xVal), y(yVal), z(zVal) {}
    int16_t x;
    int16_t y;
    int16_t z;
} Gyroscope;

typedef struct Gps {
    Gps(double timeVal = 0, double latVal = 0, double lonVal = 0, 
        double altVal = 0, uint8_t unitVal = 0, uint8_t numberOfSatVal = 0) :
        time(timeVal), lat(latVal), lon(lonVal), alt(altVal), unit(unitVal), 
        numberOfSat(numberOfSatVal) {}
    double time;
    double lat;
    double lon;
    double alt;
    uint8_t unit;
    uint8_t numberOfSat;
} Gps;

class Robot {
    public:
        Robot();
        ~Robot();

        void moveForward();
        void moveBackward();
        void turnLeft();
        void turnRight();
        
        Accelerometer& getAccelerometer();
        void setAccelerometer(const Accelerometer& newAcc);
        
        Gyroscope& getGyroscope();
        void setGyroscope(const Gyroscope & newGyro);
        
        Gps& getGps();
        void setGps(const Gps& newGps);
        
        vector<uint16_t>& getLidar();
        void setLidar(vector<uint16_t>& newLidar);

    private:
        Accelerometer accelerometer;
        Gyroscope gyroscope;
        Gps gps;
        vector<uint16_t> lidar;
};

#endif
