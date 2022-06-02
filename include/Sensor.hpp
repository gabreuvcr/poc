#ifndef SENSOR_H
#define SENSOR_H

#include <iostream>

class Sensor {
    public:
        double x, y;
        Sensor();
        Sensor(double x, double y);
        Sensor(std::pair<double, double> point);
};

#endif