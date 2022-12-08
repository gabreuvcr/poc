#include <iostream>
#include <vector>
#include <string.h>
#include <chrono>
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

    auto start_time = std::chrono::high_resolution_clock::now();

    DifferentialEvolution de = DifferentialEvolution(config, pois);
    de.run();

    auto end_time = std::chrono::high_resolution_clock::now();
    auto total_time = end_time - start_time;
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(total_time).count() << " ms" << std::endl;
    
    return 0;
}
