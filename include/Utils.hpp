#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <fstream>
#include "HarmonySearchConfig.hpp"
#include "PointOfInterest.hpp"

namespace Utils {
    void check_arguments(char *argv[], int argc, std::string &filename, bool &fixed_sensors, int &fixed_sensors_num, bool &run_tests);
    std::ifstream open_file(std::string filename);
    HarmonySearchConfig read_harmony_config(std::ifstream &file);
    double read_sensor_radius(std::ifstream &file);
    std::vector<PointOfInterest> read_pois(std::ifstream &file, int W, int H);
};

#endif