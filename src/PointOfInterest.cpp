#include "PointOfInterest.hpp"

PointOfInterest::PointOfInterest() {
    this->x = -1; this->y = -1;
}

PointOfInterest::PointOfInterest(double x, double y) {
    this->x = x; this->y = y;
}

PointOfInterest::PointOfInterest(std::pair<double, double> point) {
    this->x = point.first; this->y = point.second;
}

std::vector<PointOfInterest> PointOfInterest::generate_pois_from_grid(int W, int H, int cel_size) {
    std::vector<PointOfInterest> pois;
    for (int h = cel_size; h <= H; h += cel_size) {
        for (int w = cel_size; w <= W; w += cel_size) {
            double x = (double)(w + (w - cel_size)) / 2;
            double y = (double)(h + (h - cel_size)) / 2;
            pois.push_back(PointOfInterest(x, y));
        }
    }
    return pois;
}