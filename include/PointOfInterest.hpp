#ifndef POINT_OF_INTEREST_H
#define POINT_OF_INTEREST_H

#include <iostream>
#include <vector>

class Sensor;

class PointOfInterest {
    private:
        static std::vector<PointOfInterest> _generate_pois_from_grid(int w, int h, int cel_size);
        static int _read_type_grid(std::vector<PointOfInterest> &pois, int w, int h);
        static int _read_type_points(std::vector<PointOfInterest> &pois, int w, int h);

    public:
        double x, y;
        PointOfInterest();
        PointOfInterest(double x, double y);
        PointOfInterest(std::pair<double, double> point);

        int num_coverage(std::vector<Sensor> sensors);
        bool is_covered(Sensor sensor);

        static int read_pois(std::vector<PointOfInterest> &pois, int w, int h);
};

#endif