#include <iostream>
#include <chrono>
#include <thread>
#include <random>
#include <signal.h>
#include <condition_variable>
#include <pybind11/embed.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "robot.hpp"
#include "robot_binding.hpp"

#define CLOCK_PERIOD 1  // s
#define PACKET_PERIOD 2 // s

using namespace std;
namespace py = pybind11;

typedef enum {
    ACCELEROMETER,
    GYROSCOPE,
    GPS,
    LIDAR
} PacketType;

/**
 * @brief Generate each `sec` seconds a PacketType packet event
 * and notifies `processIA_thread` through the `pythonEvent`
 * condition variable
 * 
 * @param sec generate period
 */
void packetEvents_thread(int sec);

/**
 * @brief Generate each `sec` seconds a clock event
 * and notifies `processIA_thread` through the `pythonEvent`
 * 
 * @param sec generate period
 */
void clockEvent_thread(int sec);

/**
 * @brief Start a python intepreter, include the user IA script,
 * inject the Robot `robot` instance in the `naio` python module.
 * Wait events through the `pythonEvent` condition variable, and 
 * call the appropriate python callback
 */
void processIA_thread();

void startThreads(); 
void stopThreads();

/**
 * @brief cout thread-safe wrapper using the `stdoutMutex` mutex
 * 
 * @param str Text to log
 */
void log(const string& str);

/**
 * @brief Robot pointer shared between C++ and Python
 */
shared_ptr<Robot> robot = make_shared<Robot>();

// Thread variables
atomic<bool> threadStarted(false);

thread clockEventThread;
thread packetEventsThread;
thread processIAThread;

// Mutexes and condition variables
mutex stdoutMutex;

mutex pythonMutex;
condition_variable pythonState;
condition_variable pythonEvent;

bool pythonReady(false);
atomic<bool> clockTriggered(false);
atomic<bool> accPacketRecv(false);
atomic<bool> gyroPacketRecv(false);
atomic<bool> gpsPacketRecv(false);
atomic<bool> lidarPacketRecv(false);

// Random service variables
random_device rd;                        // only used once to initialise (seed) engine
mt19937 rng(rd());                       // random-number engine used (Mersenne-Twister in this case)

void packetEvents_thread(int sec) 
{
    uniform_int_distribution<int> randPacket(0, 3); 

    unique_lock<mutex> pythonLock(pythonMutex);
    pythonState.wait(pythonLock, []() { 
        return (!threadStarted || pythonReady); 
    });

    pythonLock.unlock();

    while (threadStarted)
    {
        PacketType packetType = static_cast<PacketType>(randPacket(rng));

        switch (packetType) {
            case ACCELEROMETER:
                accPacketRecv = true;
                break;

            case GYROSCOPE:
                gyroPacketRecv = true;
                break;

            case GPS:
                gpsPacketRecv = true;
                break;

            case LIDAR:
                lidarPacketRecv = true;
                break;

            default:
                break;
        }

        this_thread::sleep_for(chrono::seconds(sec));
    }
}

void clockEvent_thread(int sec) 
{
    unique_lock<mutex> pythonLock(pythonMutex);
    pythonState.wait(pythonLock, []() { 
        return !threadStarted || pythonReady; 
    });

    pythonLock.unlock();

    while (threadStarted)
    {
        clockTriggered = true;
        pythonEvent.notify_all();
        this_thread::sleep_for(chrono::seconds(sec));
    }
}

void processIA_thread()
{
    unique_lock<mutex> pythonLock(pythonMutex);
    py::scoped_interpreter guard{};

    // Python initialization and IA execution; End thread on any python error
    try {
        py::module naioModule    = py::module::import("naio");
        naioModule.attr("robot") = py::cast(robot);
        py::module scriptModule  = py::module::import("script");

        // Python function callbacks
        py::object initIA        = scriptModule.attr("main");
        py::object onClock       = scriptModule.attr("onClock");
        py::object onAccPacket   = scriptModule.attr("onAccPacket");
        py::object onGyroPacket  = scriptModule.attr("onGyroPacket");
        py::object onGpsPacket   = scriptModule.attr("onGpsPacket");
        py::object onLidarPacket = scriptModule.attr("onLidarPacket");

        // Init IA and dispatch python ready event
        initIA();
        pythonReady = true;
        pythonState.notify_all();

        // Wait for event and execute callback
        while (threadStarted) 
        {
            pythonEvent.wait(pythonLock, []() {
                return !threadStarted 
                    || (clockTriggered || accPacketRecv || gyroPacketRecv || gpsPacketRecv || lidarPacketRecv);
            });

            // As python function are likely to print output, lock stdout
            lock_guard<mutex> stdoutLock(stdoutMutex); 

            if (clockTriggered) {
                clockTriggered = false;
                onClock();
            } else if (accPacketRecv) {
                accPacketRecv = false;
                onAccPacket();  
            } else if (gyroPacketRecv) {
                gyroPacketRecv = false;  
                onGyroPacket(); 
            } else if (gpsPacketRecv) {
                gpsPacketRecv = false; 
                onGpsPacket(); 
            } else if (lidarPacketRecv) {
                lidarPacketRecv = false; 
                onLidarPacket();
            }
        }
    } catch(exception const& e) {
        lock_guard<mutex> stdoutLock(stdoutMutex); 
        cout << "Python error: " << e.what() << endl;
    }
}

void startThreads() 
{
    threadStarted = true;
    clockEventThread   = thread(clockEvent_thread, CLOCK_PERIOD);
    packetEventsThread = thread(packetEvents_thread, PACKET_PERIOD);
    processIAThread    = thread(processIA_thread);
}

void stopThreads() 
{
    threadStarted = false;
    pythonState.notify_all();
    pythonEvent.notify_all();

    clockEventThread.join();
    packetEventsThread.join();
    processIAThread.join();
}

void log(const string& str)
{
    lock_guard<mutex> stdoutLock(stdoutMutex);
    cout << "[CPP] " << str << endl;
}

int main()
{
    // CTRL+C handler
    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = [](int s) {
        log("Interrupted. Gracefully stopping the threads ...");
        stopThreads();
        exit(1);
    };
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);

    log("Starting threads...");
    startThreads();

    this_thread::sleep_for(chrono::seconds(10));

    log("Stopping threads...");
    stopThreads();

    return 0;
}
