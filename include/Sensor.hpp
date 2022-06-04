#ifndef SENSOR_H
#define SENSOR_H

#include <iostream>
#include <vector>

class PointOfInterest;

class Sensor {
    public:
        double x, y;
        bool active;
        static double sensor_radius;
        static double sensor_uncertainty_radius;
        
        Sensor();
        Sensor(double x, double y);
        Sensor(std::pair<double, double> point);

        bool cover(PointOfInterest poi);

        static double min_dist(std::vector<Sensor> sensors, int W, int H);
        static double dist(Sensor s1, Sensor s2);
};

#endif