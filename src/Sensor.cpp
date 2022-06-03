#include "Sensor.hpp"
#include "PointOfInterest.hpp"

double Sensor::sensor_radius = 0;

Sensor::Sensor() {
    this->x = -1; this->y = -1;
}

Sensor::Sensor(double x, double y) {
    this->x = x; this->y = y;
}

Sensor::Sensor(std::pair<double, double> point) {
    this->x = point.first; this->y = point.second;
}

bool Sensor::cover(PointOfInterest poi) {
    return (poi.x - this->x) * (poi.x - this->x) + (poi.y - this->y) * (poi.y - this->y) <= sensor_radius * sensor_radius;
}
