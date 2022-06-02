#ifndef POINT_OF_INTEREST_H
#define POINT_OF_INTEREST_H

#include <iostream>
#include <vector>

class PointOfInterest {
    public:
        double x, y;
        PointOfInterest();
        PointOfInterest(double x, double y);
        PointOfInterest(std::pair<double, double> point);

        static std::vector<PointOfInterest> generate_pois_from_grid(int W, int H, int cel_size);
        static int read_pois(std::vector<PointOfInterest> &POIs, int W, int H);
};

#endif