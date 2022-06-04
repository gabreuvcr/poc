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

    this->worst_sensor_index = -1;
    this->best_sensor_index = -1;
}

double HarmonySearch::objective(int sensors_index) {
    int n_sensors = num_sensors[sensors_index];
    double c_ratio = coverage_ratios[sensors_index];
    double min_dist_between_sensors = Sensor::min_dist(HM[sensors_index], config.W, config.H);
    const int k_const = 157;

    return ((double)1/n_sensors) * c_ratio * min_dist_between_sensors * k_const;
}

double calc_coverage_ratio(std::vector<Sensor> sensors, std::vector<PointOfInterest> pois) {
    int num_covered_pois = 0;
    for (PointOfInterest p : pois) {
        if (p.is_covered_by_at_least_one(sensors)) num_covered_pois++;
    }
    return (double)num_covered_pois / pois.size();
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

        num_sensors.push_back(hm_vector_size);
        coverage_ratios.push_back(calc_coverage_ratio(HM[vec], config.pois));
        objectives.push_back(objective(vec));

        if (worst_sensor_index == -1 || objectives[vec] < objectives[worst_sensor_index]) {
            worst_sensor_index = vec;
        }
        if (best_sensor_index == -1 || objectives[vec] > objectives[best_sensor_index]) {
            best_sensor_index = vec;
        }
    }
}

void HarmonySearch::cout_harmony_memory() {
    std::cout.precision(4);
    std::cout << std::fixed;
    
    std::cout << "min_senors: " << min_sensors << std::endl;
    std::cout << "max_sensors: " << max_sensors << std::endl;

    for (int vec = 0; vec < HM.size(); vec++) {
        std::cout << "> " << num_sensors[vec] << " sensors: " << std::endl;
        for (int s = 0; s < HM[vec].size(); s++) {
            double x = HM[vec][s].x;
            double y = HM[vec][s].y;
            if (x == -1 || y == -1) {
                std::cout << "." << " ";
            } else {
                std::cout << "(" << x << ", " << y << ") ";
            }
            if (s % 5 == 0) std::cout << std::endl;
        }
        std::cout << std::endl << std::endl;
    }
    
    for (int vec = 0; vec < HM.size(); vec++) {
        std::cout << "coverage ratios: " << coverage_ratios[vec] << " ";
        std::cout << "sensors: " << num_sensors[vec] << " ";
        std::cout << "objective: " << objectives[vec] << " ";
        if (vec == best_sensor_index) std::cout << "<best>";
        if (vec == worst_sensor_index) std::cout << "<worst>";
        std::cout << std::endl;
    }
}
