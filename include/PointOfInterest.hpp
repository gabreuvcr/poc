#ifndef POINT_OF_INTEREST_H
#define POINT_OF_INTEREST_H

#include <iostream>
#include <vector>

class Sensor;

class PointOfInterest {
    private:
        static int read_grid_type(std::vector<PointOfInterest> &pois, int w, int h);
        static int read_points_type(std::vector<PointOfInterest> &pois, int w, int h);

    public:
        double x, y;
        PointOfInterest();
        PointOfInterest(double x, double y);

        bool is_covered(Sensor sensor);
        bool is_covered_by_at_least_one(std::vector<Sensor> sensors);

        static int read_pois(std::vector<PointOfInterest> &pois, int w, int h);
        static void cout_pois(std::vector<PointOfInterest> pois);
};

#endif