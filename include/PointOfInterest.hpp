#ifndef POI_H
#define POI_H

#include <iostream>
#include <vector>

class PointOfInterest {
    public:
        double x, y;
        PointOfInterest();
        PointOfInterest(double x, double y);
        PointOfInterest(std::pair<double, double> point);

        static std::vector<PointOfInterest> generate_pois_from_grid(int W, int H, int cel_size);
};

#endif