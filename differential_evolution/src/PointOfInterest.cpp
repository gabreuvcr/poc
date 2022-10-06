#include <cmath>
#include "PointOfInterest.hpp"
#include "Sensor.hpp"
#include "Constants.hpp"

PointOfInterest::PointOfInterest(double x, double y) {
    this->x = x; this->y = y;
}

double PointOfInterest::distance_to(Sensor sensor) {
    return sqrt(
        (sensor.x - this->x) * (sensor.x - this->x) + 
        (sensor.y - this->y) * (sensor.y - this->y)
    );
}

double probability_function(double dist_to_sensor) {
    double L1 = Sensor::radius_err - Sensor::radius + dist_to_sensor;
    double L2 = Sensor::radius_err + Sensor::radius - dist_to_sensor;

    return exp(-Constants::A1 * pow(L1, Constants::B1) / pow(L2, Constants::B2) + Constants::A2);
}

double PointOfInterest::probability_coverage(Sensor sensor) {
    double dist_to_sensor = this->distance_to(sensor);

    if (dist_to_sensor <= Sensor::min_radius) {
        return 1;
    } else if (dist_to_sensor < Sensor::max_radius) {
        return probability_function(dist_to_sensor);
    } else {
        return 0;
    }
}

double PointOfInterest::joint_coverage(std::vector<Sensor> sensors, std::vector<bool> active_sensors) {
    double mult = 1;

    for (int s = 0; s < sensors.size(); s++) {
        if (active_sensors[s]) {
            mult *= 1 - this->probability_coverage(sensors[s]);
        }
    }

    return 1 - mult;
}

void PointOfInterest::cout_pois(std::vector<PointOfInterest> pois) {
    std::cout << pois.size() << std::endl;
    for (PointOfInterest poi : pois) {
        std::cout << "(" << poi.x << ", " << poi.y << ") ";
    }
    std::cout << std::endl;
}
