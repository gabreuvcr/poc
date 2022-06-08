#include "PointOfInterest.hpp"
#include "Sensor.hpp"

PointOfInterest::PointOfInterest() {
    this->x = -1; this->y = -1;
}

PointOfInterest::PointOfInterest(double x, double y) {
    this->x = x; this->y = y;
}

PointOfInterest::PointOfInterest(std::pair<double, double> point) {
    this->x = point.first; this->y = point.second;
}

bool PointOfInterest::is_covered(Sensor sensor) {
    return (sensor.x - x) * (sensor.x - x) + (sensor.y - y) * (sensor.y - y) <= sensor.radius * sensor.radius;
}

bool PointOfInterest::is_covered_by_at_least_one(std::vector<Sensor> sensors) {
    for (Sensor sensor : sensors) {
        if (is_covered(sensor)) return true;
    }
    return false;
}

std::vector<PointOfInterest> PointOfInterest::_generate_pois_from_grid(int W, int H, int cel_size) {
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

int PointOfInterest::_read_type_grid(std::vector<PointOfInterest> &pois, int W, int H) {
    int cel_size; std::cin >> cel_size;
    // std::cout << W << " width" << std::endl;
    // std::cout << H << " height" << std::endl;
    // std::cout << "cel_size: " << cel_size << std::endl;

    if (W % cel_size != 0 || H % cel_size != 0 || cel_size < 1 || cel_size > W || cel_size > H) {
        std::cout << "Invalid cel_size value" << std::endl;
        return -1;
    }

    pois = PointOfInterest::_generate_pois_from_grid(W, H, cel_size);

    // std::cout << "Generated " << pois.size() << " pois" << std::endl;
    return 1;
}

int PointOfInterest::_read_type_points(std::vector<PointOfInterest> &pois, int W, int H) {
    int num_pois; std::cin >> num_pois;

    for (int p = 0; p < num_pois; p++) {
        double x, y; std::cin >> x >> y;
        if (x < 0 || x > W || y < 0 || y > H) {
            std::cout << "Invalid coordinate value" << std::endl;
            return -1;
        }
        pois.push_back(PointOfInterest(x, y));
    }
    // std::cout << "Read " << pois.size() << " pois" << std::endl;
    return 1;
}

int PointOfInterest::read_pois(std::vector<PointOfInterest> &pois, int W, int H) {
    std::string type; std::cin >> type;

    if (type != "G" && type != "P") return -1;

    if (type == "G") {
        return PointOfInterest::_read_type_grid(pois, W, H);
    } else if (type == "P") {
        return PointOfInterest::_read_type_points(pois, W, H);
    }

    return -1;
}
