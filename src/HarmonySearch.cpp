#include <cmath>

#include "HarmonySearch.hpp"

HarmonySearch::HarmonySearch(
    int W,
    int H,
    int HM_size,
    double sensor_radius,
    double sensor_uncertainty_radius,
    std::vector<PointOfInterest> POIs
) {
    this->W = W;
    this->H = H;
    this->HM_size = HM_size;
    this->sensor_radius = sensor_radius;
    this->sensor_uncertainty_radius = sensor_uncertainty_radius;
    this->POIs = POIs;

    double max_radius = sensor_radius + sensor_uncertainty_radius;
    double min_radius = sensor_radius - sensor_uncertainty_radius;

    this->x_lower = min_radius;
    this->y_lower = min_radius;
    this->x_upper = W - (min_radius);
    this->y_upper = H - (min_radius);

    this->min_sensors = ceil((W / (2 * max_radius)) * (H / (2 * max_radius)));
    this->max_sensors = ceil((W / (2 * min_radius)) * (H / (2 * min_radius)));
}

void HarmonySearch::init_harmony_memory() {
    HM.resize(this->HM_size, std::vector<Sensor>(this->max_sensors, Sensor()));

    for (int vec = 0; vec < this->HM.size(); vec++) {
        int HM_vector_size = Random::random_value(this->min_sensors, this->max_sensors);

        for (int s = 0; s < HM_vector_size; s++) {
            double x = Random::random_value(this->x_lower, this->x_upper);
            double y = Random::random_value(this->y_lower, this->y_upper);
            this->HM[vec][s] = Sensor(x, y);
        }
        std::shuffle(begin(this->HM[vec]), end(this->HM[vec]), Random::rng);
        this->num_sensors.push_back(HM_vector_size);
    }
}

void HarmonySearch::cout_harmony_memory() {
    std::cout.precision(1);
    std::cout << std::fixed;
    
    std::cout << "min_senors: " << this->min_sensors << std::endl;
    std::cout << "max_sensors: " << this->max_sensors << std::endl;

    for (int vec = 0; vec < this->HM.size(); vec++) {
        std::cout << "> " << this->num_sensors[vec] << " sensors: " << std::endl;
        for (int s = 0; s < this->HM[vec].size(); s++) {
            double x = this->HM[vec][s].x;
            double y = this->HM[vec][s].y;
            if (x == -1 || y == -1) {
                std::cout << "." << " ";
            } else {
                std::cout << "(" << x << ", " << y << ") ";
            }
            if (s % 5 == 0) std::cout << std::endl;
        }
        std::cout << std::endl << std::endl;
    }
}
