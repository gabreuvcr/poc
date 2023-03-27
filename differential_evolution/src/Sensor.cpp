#include <cmath>
#include "Sensor.hpp"
#include "Random.hpp"

double Sensor::radius = 0, Sensor::radius_err = 0;
double Sensor::max_radius = 0, Sensor::min_radius = 0;
double Sensor::x_lower = 0, Sensor::x_upper = 0;
double Sensor::y_lower = 0, Sensor::y_upper = 0;

Sensor::Sensor() {
    this->x = 0; this->y = 0;
}

Sensor::Sensor(double x, double y) {
    this->x = x; this->y = y;
}

void Sensor::set_values(double radius, double radius_err, int W, int H) {
    Sensor::radius = radius;
    Sensor::radius_err = radius_err;

    Sensor::max_radius = radius + radius_err;
    Sensor::min_radius = radius - radius_err;

    Sensor::x_lower = Sensor::y_lower = Sensor::min_radius;
    Sensor::x_upper = W - Sensor::min_radius;
    Sensor::y_upper = H - Sensor::min_radius;
}

// double Sensor::distance_to(Sensor s) {
//     return sqrt(
//         (this->x - s.x) * (this->x - s.x) + 
//         (this->y - s.y) * (this->y - s.y)
//     );
// }

Sensor Sensor::random_sensor() {
    double x = Random::random_value(x_lower, x_upper);
    double y = Random::random_value(y_lower, y_upper);

    return Sensor(x, y);
}

void Sensor::cout_sensors(std::vector<Sensor> sensors) {
    for (int s = 0; s < sensors.size(); s++) {
        std::cout << "(" << sensors[s].x << ", " << sensors[s].y << ") ";
    }
    std::cout << std::endl;
}

void Sensor::cout_sensors(std::vector<Sensor> sensors, std::vector<bool> active_sensors) {
    for (int s = 0; s < sensors.size(); s++) {
        if (active_sensors[s]) {
            std::cout << "(" << sensors[s].x << ", " << sensors[s].y << ") ";
        } else {
            std::cout << ". ";
        }
    }
    std::cout << std::endl;
}
