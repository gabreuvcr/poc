#include <cmath>

#include "HarmonySearch.hpp"
#include "Random.hpp"

HarmonySearch::HarmonySearch(HarmonySearchConfig config) {
    this->config = config;

    double max_radius = config.sensor_radius + config.sensor_uncertainty_radius;
    double min_radius = config.sensor_radius - config.sensor_uncertainty_radius;

    this->x_lower = min_radius;
    this->y_lower = min_radius;
    this->x_upper = config.W - (min_radius);
    this->y_upper = config.H - (min_radius);

    this->min_sensors = ceil((config.W / (2 * max_radius)) * (config.H / (2 * max_radius)));
    this->max_sensors = ceil((config.W / (2 * min_radius)) * (config.H / (2 * min_radius)));
}

double HarmonySearch::objective(int sensors_index) {
    int n_sensors = num_sensors[sensors_index];
    int n_k_coverage = num_k_coverage[sensors_index];

    return ((double)1/n_sensors) * n_k_coverage;
}

int min_coverage(std::vector<Sensor> sensors, std::vector<PointOfInterest> poi) {
    int min_coverage = 0x3F3F3F3F;
    for (PointOfInterest p : poi) {
        int covered = p.num_coverage(sensors);
        min_coverage = std::min(min_coverage, covered);
    }

    return min_coverage;
}

void HarmonySearch::init_harmony_memory() {
    HM.resize(config.hm_size, std::vector<Sensor>(max_sensors, Sensor()));

    for (int vec = 0; vec < HM.size(); vec++) {
        int hm_vector_size = Random::random_value(min_sensors, max_sensors);

        for (int s = 0; s < hm_vector_size; s++) {
            double x = Random::random_value(x_lower, x_upper);
            double y = Random::random_value(y_lower, y_upper);
            HM[vec][s] = Sensor(x, y);
        }
        std::shuffle(begin(HM[vec]), end(HM[vec]), Random::rng);


        int min_k_coverage = min_coverage(HM[vec], config.pois);
        num_k_coverage.push_back(min_k_coverage);
        num_sensors.push_back(hm_vector_size);
        objectives.push_back(objective(vec));
    }
}

void HarmonySearch::cout_harmony_memory() {
    std::cout.precision(4);
    std::cout << std::fixed;
    
    std::cout << "min_senors: " << min_sensors << std::endl;
    std::cout << "max_sensors: " << max_sensors << std::endl;

    // for (int vec = 0; vec < HM.size(); vec++) {
    //     std::cout << "> " << num_sensors[vec] << " sensors: " << std::endl;
    //     for (int s = 0; s < HM[vec].size(); s++) {
    //         double x = HM[vec][s].x;
    //         double y = HM[vec][s].y;
    //         if (x == -1 || y == -1) {
    //             std::cout << "." << " ";
    //         } else {
    //             std::cout << "(" << x << ", " << y << ") ";
    //         }
    //         if (s % 5 == 0) std::cout << std::endl;
    //     }
    //     std::cout << std::endl << std::endl;
    // }
    
    for (int vec = 0; vec < HM.size(); vec++) {
        std::cout << "k-coverage: " << num_k_coverage[vec] << " " << "sensors: " << num_sensors[vec] << " " << "objective: " << objectives[vec] << std::endl;
    }
}
