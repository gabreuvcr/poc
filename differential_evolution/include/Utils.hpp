#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <fstream>
#include "DifferentialEvolutionConfig.hpp"
#include "PointOfInterest.hpp"

namespace Utils {
    void check_arguments(char *argv[], int argc, std::string &filename, bool &all);
    std::ifstream open_file(std::string filename);
    DifferentialEvolutionConfig read_differential_evolution_config(std::ifstream &file);
    double read_sensor_radius(std::ifstream &file);
    std::vector<PointOfInterest> read_pois(std::ifstream &file, int W, int H);
};

#endif