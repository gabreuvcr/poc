#include "PointOfInterest.hpp"
#include "Sensor.hpp"

PointOfInterest::PointOfInterest() {
    this->x = -1; this->y = -1;
}

PointOfInterest::PointOfInterest(double x, double y) {
    this->x = x; this->y = y;
}

//To-do: change to probability function
bool PointOfInterest::is_covered(Sensor sensor) {
    return (sensor.x - this->x) * (sensor.x - this->x) + (sensor.y - this->y) * (sensor.y - this->y) <= sensor.radius * sensor.radius;
}

bool PointOfInterest::is_covered_by_at_least_one(std::vector<Sensor> sensors) {
    for (Sensor sensor : sensors) {
        if (this->is_covered(sensor)) return true;
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

int PointOfInterest::_read_grid_type(std::vector<PointOfInterest> &pois, int W, int H) {
    int cel_size; std::cin >> cel_size;

    if (W % cel_size != 0 || H % cel_size != 0 || cel_size < 1 || cel_size > W || cel_size > H) {
        throw std::runtime_error("Invalid cel_size value");
    }

    pois = PointOfInterest::_generate_pois_from_grid(W, H, cel_size);

    return 1;
}

int PointOfInterest::_read_points_type(std::vector<PointOfInterest> &pois, int W, int H) {
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
        return PointOfInterest::_read_grid_type(pois, W, H);
    } else if (type == "P") {
        return PointOfInterest::_read_points_type(pois, W, H);
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
