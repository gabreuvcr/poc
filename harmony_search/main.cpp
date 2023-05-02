#include <iostream>
#include <vector>
#include "HarmonySearch.hpp"
#include "HarmonySearchConfig.hpp"
#include "PointOfInterest.hpp"
#include "Utils.hpp"
#include "Sensor.hpp"
#include "Experiment.hpp"

int main(int argc, char* argv[]) {
    std::string filename;
    bool fixed_sensors = false; int num_fixed_sensors = 0;
    bool test = false;

    Utils::check_arguments(argv, argc, filename, test, fixed_sensors, num_fixed_sensors);

    std::ifstream file = Utils::open_file(filename);

    HarmonySearchConfig config = Utils::read_harmony_config(file);

    double radius = Utils::read_sensor_radius(file);
    double radius_err = radius * 0.5;

    std::vector<PointOfInterest> pois = Utils::read_pois(file, config.W, config.H);

    Sensor::set_values(radius, radius_err, config.W, config.H);
    
    if (test) {
        Experiment::run_test(config, pois);
    } else if (fixed_sensors) {
        HarmonySearch hs = HarmonySearch(config, pois, num_fixed_sensors);
        double curr_coverage = hs.run();
        std::cout << num_fixed_sensors << ": " << curr_coverage << std::endl;
    }

    return 0;
}
