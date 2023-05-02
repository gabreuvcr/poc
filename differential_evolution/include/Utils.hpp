#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <fstream>
#include <chrono>
#include "DifferentialEvolutionConfig.hpp"
#include "PointOfInterest.hpp"
#include "Agent.hpp"

namespace Utils {
    void log_hyper_volume(double hyper_volume);
    void log_population(std::vector<Agent> population);
    void log_time(std::chrono::duration<double> time);
    void log_seed(int seed);
    void check_arguments(char *argv[], int argc, std::string &filename, bool &test);
    std::ifstream open_file(std::string filename);
    DifferentialEvolutionConfig read_differential_evolution_config(std::ifstream &file);
    double read_sensor_radius(std::ifstream &file);
    std::vector<PointOfInterest> read_pois(std::ifstream &file, int W, int H);
};

#endif