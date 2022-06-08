#include <cmath>

#include "HarmonySearch.hpp"
#include "Constants.hpp"
#include "Random.hpp"

HarmonySearch::HarmonySearch(HarmonySearchConfig config) {
    this->config = config;

    this->min_sensors = ceil(
        (config.W / (2 * Sensor::max_radius)) * (config.H / (2 * Sensor::max_radius))
    );
    this->max_sensors = ceil(
        (config.W / (2 * Sensor::min_radius)) * (config.H / (2 * Sensor::min_radius))
    );

    this->worst_sensors_index = -1;
    this->best_sensors_index = -1;
}

void HarmonySearch::starts() {
    this->init_harmony_memory();
    int iteration = 0;
    std::cout << "Start: " << std::endl;
    this->cout_harmony_memory();
    while (iteration < config.num_iterations) {
        std::vector<Sensor> new_vector(max_sensors, Sensor());
        int count_sensors = 0;

        for (int s = 0; s < max_sensors; s++) {
            
            if (Random::random() <= Constants::HMCR) {
                //Memory Consideration
                int i = Random::random_value(0, HM.size() - 1);
                Sensor hm_sensor = HM[i][s];

                if (hm_sensor.active && Random::random() <= Constants::PAR) {
                    //Pitch Adjustment
                    if (Random::random() < 0.5) {
                        hm_sensor.x += (Random::random() * Constants::BW);
                    } else {
                        hm_sensor.x -= (Random::random() * Constants::BW);
                    } 
                    
                    if (Random::random() < 0.5) {
                        hm_sensor.y += (Random::random() * Constants::BW);
                    } else {
                        hm_sensor.y -= (Random::random() * Constants::BW);
                    }
                }
                new_vector[s] = hm_sensor;
            } else { 
                //Random Consideration
                double x = Random::random_value(Sensor::x_lower, Sensor::x_upper);
                double y = Random::random_value(Sensor::y_lower, Sensor::y_upper);
                new_vector[s] = Sensor(x, y);
            }

            if (new_vector[s].active) count_sensors++;
        }
        if (count_sensors >= min_sensors) {
            double coverage_ratio = calculate_coverage_ratio(new_vector, config.pois);
            double new_objective = calculate_objective(new_vector, count_sensors, coverage_ratio);
            
            if (new_objective > objectives[worst_sensors_index]) {
                //Update the worst sensor with the new vector
                HM[worst_sensors_index] = new_vector;
                objectives[worst_sensors_index] = new_objective;
                num_sensors[worst_sensors_index] = count_sensors;
                coverage_ratios[worst_sensors_index] = coverage_ratio;
                update_best_and_worst_index();
            }
        }

        iteration++;
    }
    std::cout << std::endl;
    this->cout_harmony_memory();
    std::cout << std::endl;
}

double HarmonySearch::calculate_objective(std::vector<Sensor> sensors, int n_sensors, double c_ratio) {
    double min_dist_between_sensors = Sensor::min_dist(sensors, config.W, config.H);
    const int k_const = 128;

    return ((double)1/n_sensors) + c_ratio * min_dist_between_sensors * k_const;
    // return ((double)1/n_sensors) * pow(c_ratio, 3) * min_dist_between_sensors * k_const;
}

void HarmonySearch::update_best_and_worst_index() {
    int best_index = -1;
    int worst_index = -1;
    for (int vec = 0; vec < HM.size(); vec++) {
        if (best_index == -1 || objectives[vec] > objectives[best_index]) {
            best_index = vec;
        }
        if (worst_index == -1 || objectives[vec] < objectives[worst_index]) {
            worst_index = vec;
        }
    }
    this->best_sensors_index = best_index;
    this->worst_sensors_index = worst_index;
}

double HarmonySearch::calculate_coverage_ratio(std::vector<Sensor> sensors, std::vector<PointOfInterest> pois) {
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
            double x = Random::random_value(Sensor::x_lower, Sensor::x_upper);
            double y = Random::random_value(Sensor::y_lower, Sensor::y_upper);
            HM[vec][s] = Sensor(x, y);
        }
        std::shuffle(begin(HM[vec]), end(HM[vec]), Random::rng);

        num_sensors.push_back(hm_vector_size);
        coverage_ratios.push_back(calculate_coverage_ratio(HM[vec], config.pois));
        objectives.push_back(calculate_objective(HM[vec], num_sensors[vec], coverage_ratios[vec]));

        if (worst_sensors_index == -1 || objectives[vec] < objectives[worst_sensors_index]) {
            worst_sensors_index = vec;
        }
        if (best_sensors_index == -1 || objectives[vec] > objectives[best_sensors_index]) {
            best_sensors_index = vec;
        }
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
        std::cout << "coverage ratios: " << coverage_ratios[vec] << " | ";
        std::cout << "sensors: " << num_sensors[vec] << " | ";
        std::cout << "objective: " << objectives[vec] << " | ";
        if (vec == best_sensors_index) std::cout << "<best>";
        if (vec == worst_sensors_index) std::cout << "<worst>";
        std::cout << std::endl;
    }
}
