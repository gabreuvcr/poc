#ifndef SENSOR_H
#define SENSOR_H

#include <iostream>
#include <vector>

class Sensor {
    private:
    public:
        static double radius, radius_err;
        static double max_radius, min_radius;
        static double x_lower, x_upper;
        static double y_lower, y_upper;

        double x, y;
        bool active;
        
        Sensor();
        Sensor(double x, double y);

        double distance(Sensor s);

        static void set_values(double radius, double radius_err, int W, int H);
        static double min_dist(std::vector<Sensor> sensors, int W, int H);
        static Sensor random_sensor();
        static void cout_sensors(std::vector<Sensor> sensors);
};

#endif