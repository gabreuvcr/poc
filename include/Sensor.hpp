#ifndef SENSOR_H
#define SENSOR_H

#include <iostream>

class PointOfInterest;

class Sensor {
    public:
        double x, y;
        static double sensor_radius;
        Sensor();
        Sensor(double x, double y);
        Sensor(std::pair<double, double> point);

        bool cover(PointOfInterest poi);
};

#endif