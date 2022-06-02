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
    std::vector<PointOfInterest> POIs;
    for (int h = cel_size; h <= H; h += cel_size) {
        for (int w = cel_size; w <= W; w += cel_size) {
            double x = (double)(w + (w - cel_size)) / 2;
            double y = (double)(h + (h - cel_size)) / 2;
            POIs.push_back(PointOfInterest(x, y));
        }
    }
    return POIs;
}

int PointOfInterest::read_pois(std::vector<PointOfInterest> &POIs, int W, int H) {
    std::string type; std::cin >> type;

    if (type != "G" && type != "P") return -1;

    if (type == "G") {
        int cel_size; std::cin >> cel_size;

        if (W % cel_size != 0 || H % cel_size != 0 || cel_size < 1 || cel_size > W || cel_size > H) {
            std::cout << "Invalid cel_size value" << std::endl;
            return -1;
        }

        POIs = PointOfInterest::generate_pois_from_grid(W, H, cel_size);

        std::cout << "Generated " << POIs.size() << " POIs" << std::endl;

    } else if (type == "P") {
        int num_pois; std::cin >> num_pois;

        for (int p = 0; p < num_pois; p++) {
            double x, y; std::cin >> x >> y;
            if (x < 0 || x > W || y < 0 || y > H) {
                std::cout << "Invalid coordinate value" << std::endl;
                return -1;
            }
            POIs.push_back(PointOfInterest(x, y));
        }
        std::cout << "Read " << POIs.size() << " POIs" << std::endl;

    }

    return 1;
}