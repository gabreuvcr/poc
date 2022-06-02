#include "Sensor.hpp"

Sensor::Sensor() {
    this->x = -1; this->y = -1;
}

Sensor::Sensor(double x, double y) {
    this->x = x; this->y = y;
}

Sensor::Sensor(std::pair<double, double> point) {
    this->x = point.first; this->y = point.second;
}
