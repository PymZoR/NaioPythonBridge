from naio import *

# IA entry point
def main():
    print "[IA] started"

# Clock event callback
def onClock():
    print "[IA] clock"

# Accelerometer packet callback
def onAccPacket():
    acc = robot.accelerometer
    print "[IA] Acc packet:", acc.x, acc.y, acc.z

# Gyroscope packet callback
def onGyroPacket():
    gyro = robot.gyroscope
    print "[IA] Gyro packet:", gyro.x, gyro.y, gyro.z

# GPS packet callback
def onGpsPacket():
    gps = robot.gps
    print "[IA] Gps packet:", gps.lat, gps.lon, gps.alt, gps.numberOfSat

# Lidar packet callback
def onLidarPacket():
    lidar = robot.lidar
    print "[IA] Lidar packet"
    print lidar

