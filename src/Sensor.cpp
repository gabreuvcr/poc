#include <cmath>

#include "Sensor.hpp"
#include "PointOfInterest.hpp"
#include "Random.hpp"

double Sensor::radius = 0, Sensor::radius_err = 0;
double Sensor::max_radius = 0, Sensor::min_radius = 0;
double Sensor::x_lower = 0, Sensor::x_upper = 0;
double Sensor::y_lower = 0, Sensor::y_upper = 0;

Sensor::Sensor() {
    this->active = false;
    this->x = -1; this->y = -1;
}

Sensor::Sensor(double x, double y) {
    this->active = true;
    this->x = x; this->y = y;
}

void Sensor::set_values(int radius, int radius_err, int W, int H) {
    Sensor::radius = radius;
    Sensor::radius_err = radius_err;

    Sensor::max_radius = radius + radius_err;
    Sensor::min_radius = radius - radius_err;

    Sensor::x_lower = Sensor::y_lower = Sensor::min_radius;
    Sensor::x_upper = W - Sensor::min_radius;
    Sensor::y_upper = H - Sensor::min_radius;
}

double Sensor::distance(Sensor s) {
    return sqrt((this->x - s.x) * (this->x - s.x) + (this->y - s.y) * (this->y - s.y));
}

double Sensor::min_dist(std::vector<Sensor> sensors, int W, int H) {
    int Wm = W - (Sensor::radius - Sensor::radius_err);
    int Hm = H - (Sensor::radius - Sensor::radius_err);

    double max_dist_sensors = sqrt(Wm * Wm + Hm * Hm);
    double min_dist_sensors = max_dist_sensors;

    if (max_dist_sensors == 0) return 1;

    for (int si = 0; si < sensors.size(); si++) {
        for (int sj = 0; sj < sensors.size(); sj++) {
            if (si == sj || !sensors[si].active || !sensors[sj].active) continue;

            double dist = sensors[si].distance(sensors[sj]);

            min_dist_sensors = std::min(min_dist_sensors, dist);
        }
    }

    return min_dist_sensors / max_dist_sensors;
}

Sensor Sensor::random_sensor() {
    double x = Random::random_value(x_lower, x_upper);
    double y = Random::random_value(y_lower, y_upper);

    return Sensor(x, y);
}

void Sensor::cout_sensors(std::vector<Sensor> sensors) {
    for (Sensor sensor : sensors) {
        if (!sensor.active) {
            std::cout << ". ";
        } else {
            std::cout << "(" << sensor.x << ", " << sensor.y << ") ";
        }
    }
}
