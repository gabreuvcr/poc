#ifndef POINT_OF_INTEREST_H
#define POINT_OF_INTEREST_H

#include <iostream>
#include <vector>

class Sensor;

class PointOfInterest {
    private:
        double probability_coverage(Sensor sensor);
        double distance_to(Sensor sensor);

    public:
        double x, y;

        PointOfInterest(double x, double y);
        double joint_coverage(std::vector<Sensor> sensors);

        static void cout_pois(std::vector<PointOfInterest> pois);
};

#endif