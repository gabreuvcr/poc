#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <fstream>
#include <chrono>
#include "HarmonySearchConfig.hpp"
#include "PointOfInterest.hpp"

namespace Utils {
    void log_population(
        int min_sensors, 
        int last_number_of_sensors, 
        std::vector<double> coverage_ratio
    );
    void log_time(std::chrono::duration<double> time);
    void log_seed(int seed);
    void check_arguments(
        char *argv[], 
        int argc, 
        std::string &filename, 
        bool &all, 
        bool &fixed_sensors, 
        int &fixed_sensors_num
    );
    std::ifstream open_file(std::string filename);
    HarmonySearchConfig read_harmony_config(std::ifstream &file);
    double read_sensor_radius(std::ifstream &file);
    std::vector<PointOfInterest> read_pois(std::ifstream &file, int W, int H);
};

#endif