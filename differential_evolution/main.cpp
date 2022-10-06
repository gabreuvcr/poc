#include <iostream>
#include <vector>
#include <string.h>
#include "DifferentialEvolution.hpp"
#include "PointOfInterest.hpp"
#include "Utils.hpp"
#include "Sensor.hpp"

int main(int argc, char* argv[]) {
    std::string filename;

    Utils::check_arguments(argv, argc, filename);

    std::ifstream file = Utils::open_file(filename);

    DifferentialEvolutionConfig config = Utils::read_differential_evolution_config(file);

    double radius = Utils::read_sensor_radius(file);

    std::vector<PointOfInterest> pois = Utils::read_pois(file, config.W, config.H);

    Sensor::set_values(radius, config.W, config.H);

    DifferentialEvolution de = DifferentialEvolution(config, pois);
    
    de.run();
    
    return 0;
}
