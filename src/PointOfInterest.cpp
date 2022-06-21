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
    if (!sensor.active) return 0;

    double dist_to_sensor = this->distance_to(sensor);

    if (dist_to_sensor <= Sensor::min_radius) {
        return 1;
    } else if (dist_to_sensor < Sensor::max_radius) {
        return probability_function(dist_to_sensor);
    } else {
        return 0;
    }
}

double PointOfInterest::joint_coverage(std::vector<Sensor> sensors) {
    double mult = 1;

    for (Sensor sensor : sensors) {
        
        mult *= 1 - this->probability_coverage(sensor);
    }

    return 1 - mult;
}

int PointOfInterest::read_grid_type(std::vector<PointOfInterest> &pois, int W, int H) {
    int cel_size; std::cin >> cel_size;

    if (W % cel_size != 0 || H % cel_size != 0 || cel_size < 1 || cel_size > W || cel_size > H) {
        throw std::runtime_error("Invalid cel_size value");
    }

    for (int h = cel_size; h <= H; h += cel_size) {
        for (int w = cel_size; w <= W; w += cel_size) {
            double x = (double)(w + (w - cel_size)) / 2;
            double y = (double)(h + (h - cel_size)) / 2;
            pois.push_back(PointOfInterest(x, y));
        }
    }
    
    return 1;
}

int PointOfInterest::read_points_type(std::vector<PointOfInterest> &pois, int W, int H) {
    int num_pois; std::cin >> num_pois;

    for (int p = 0; p < num_pois; p++) {
        double x, y; std::cin >> x >> y;

        if (x < 0 || x > W || y < 0 || y > H) {
            throw std::runtime_error("Invalid coordinate value");
        }

        pois.push_back(PointOfInterest(x, y));
    }

    return 1;
}

int PointOfInterest::read_pois(std::vector<PointOfInterest> &pois, int W, int H) {
    std::string type; std::cin >> type;

    if (type != "G" && type != "P") throw std::runtime_error("Must be type G or P");

    if (type == "G") {
        return PointOfInterest::read_grid_type(pois, W, H);
    } else if (type == "P") {
        return PointOfInterest::read_points_type(pois, W, H);
    }

    return -1;
}

void PointOfInterest::cout_pois(std::vector<PointOfInterest> pois) {
    std::cout << pois.size() << std::endl;
    for (PointOfInterest poi : pois) {
        std::cout << "(" << poi.x << ", " << poi.y << ") ";
    }
    std::cout << std::endl;
}
