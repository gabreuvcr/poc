#include <iostream>
#include <vector>
#include "DifferentialEvolution.hpp"
#include "PointOfInterest.hpp"
#include "Utils.hpp"
#include "Sensor.hpp"
#include "Agent.hpp"
#include "Experiment.hpp"

int main(int argc, char* argv[]) {
    std::string filename;
    bool test = false;

    Utils::check_arguments(argv, argc, filename, test);

    std::ifstream file = Utils::open_file(filename);

    DifferentialEvolutionConfig config = Utils::read_differential_evolution_config(file);

    double radius = Utils::read_sensor_radius(file);
    double radius_err = radius * 0.5;

    std::vector<PointOfInterest> pois = Utils::read_pois(file, config.W, config.H);

    Sensor::set_values(radius, radius_err, config.W, config.H);

    if (test) {
        Experiment::run_test(config, pois);
    } else {
        DifferentialEvolution de = DifferentialEvolution(config, pois);
        de.run();
        std::vector<Agent> pareto_front = de.get_pareto_front();
        Utils::log_population(pareto_front);
    }
    
    return 0;
}
