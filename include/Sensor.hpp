#ifndef SENSOR_H
#define SENSOR_H

#include <iostream>
#include <vector>

class PointOfInterest;

class Sensor {
    private:
    public:
        double x, y;
        bool active;
        static double radius, radius_err;
        static double max_radius, min_radius;
        static double x_lower, x_upper;
        static double y_lower, y_upper;
        
        Sensor();
        Sensor(double x, double y);
        Sensor(std::pair<double, double> point);

        bool cover(PointOfInterest poi);

        static void set_values(int radius, int radius_err, int W, int H);
        static double min_dist(std::vector<Sensor> sensors, int W, int H);
        static double dist(Sensor s1, Sensor s2);
};

#endif