#include <cmath>

#include "Sensor.hpp"
#include "PointOfInterest.hpp"


double Sensor::sensor_radius = 0;
double Sensor::sensor_uncertainty_radius = 0;

Sensor::Sensor() {
    this->active = false;
    this->x = -1; this->y = -1;
}

Sensor::Sensor(double x, double y) {
    this->active = true;
    this->x = x; this->y = y;
}

Sensor::Sensor(std::pair<double, double> point) {
    this->active = true;
    this->x = point.first; this->y = point.second;
}

bool Sensor::cover(PointOfInterest poi) {
    return (poi.x - this->x) * (poi.x - this->x) + (poi.y - this->y) * (poi.y - this->y) <= sensor_radius * sensor_radius;
}

double Sensor::dist(Sensor s1, Sensor s2) {
    return sqrt((s1.x - s2.x) * (s1.x - s2.x) + (s1.y - s2.y) * (s1.y - s2.y));
}

double Sensor::min_dist(std::vector<Sensor> sensors, int W, int H) {
    int Wm = W - (Sensor::sensor_radius - Sensor::sensor_uncertainty_radius);
    int Hm = H - (Sensor::sensor_radius - Sensor::sensor_uncertainty_radius);

    double max_dist_sensors = sqrt(Wm * Wm + Hm * Hm);
    double min_dist_sensors = max_dist_sensors;

    for (int si = 0; si < sensors.size(); si++) {
        for (int sj = 0; sj < sensors.size(); sj++) {
            if (si == sj || !sensors[si].active || !sensors[sj].active) continue;

            double dist = Sensor::dist(sensors[si], sensors[sj]);

            min_dist_sensors = std::min(min_dist_sensors, dist);
        }
    }

    return min_dist_sensors / max_dist_sensors;
}