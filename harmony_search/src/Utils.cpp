#include <fstream>
#include <string.h>
#include <vector>
#include "Utils.hpp"
#include "PointOfInterest.hpp"

namespace Utils {
    void check_arguments(char *argv[], int argc, std::string &filename, bool &fixed_sensors, int &num_fixed_sensors, bool &run_tests) {
        if (argc == 1) {
            throw std::runtime_error("Usage: " + std::string(argv[0]) + " <input_file> [-f <value>] [-t]");
            exit(1);
        }
        if (argc >= 2) {
            filename = argv[1];
        }
        if (argc >= 3) {
            for (int i = 2; i < argc; i++) {
                if (strcmp(argv[i], "-f") == 0) {
                    if (i + 1 >= argc || strcmp(argv[i + 1], "-t") == 0) {
                        throw std::runtime_error("Fixed sensors numbers is required after -f");
                    }
                    fixed_sensors = true;
                    num_fixed_sensors = atoi(argv[++i]);
                    if (num_fixed_sensors <= 0) {
                        throw std::runtime_error("Fixed sensors number must be a valid value");
                    }
                } else if (strcmp(argv[i], "-t") == 0) {
                    run_tests = true;
                }
            }
        }
    }

    std::ifstream open_file(std::string filename) {
        std::ifstream file(filename);

        if (!file.is_open()) {
            throw std::runtime_error("Error: could not open file " + filename);
        }
        
        return file;
    }

    HarmonySearchConfig read_harmony_config(std::ifstream &file) {
        HarmonySearchConfig config = HarmonySearchConfig();

        file >> config.W >> config.H;
        file >> config.mem_size >> config.num_iterations;

        return config;
    }

    double read_sensor_radius(std::ifstream &file) {
        double radius; file >> radius;
        return radius;
    }

    std::vector<PointOfInterest> read_pois(std::ifstream &file, int W, int H) {
        std::string type; file >> type;
        std::vector<PointOfInterest> pois;

        if (type != "G" && type != "P") {
            throw std::runtime_error("Must be type G or P");
        }

        if (type == "G") {
            int cel_size; file >> cel_size;

            if (W % cel_size != 0 || H % cel_size != 0 || cel_size < 1 || cel_size > W || cel_size > H) {
                throw std::runtime_error("Invalid cel_size value");
            }

            for (int h = cel_size; h <= H; h += cel_size) {
                for (int w = cel_size; w <= W; w += cel_size) {
                    double x = (double)(w + (w - cel_size)) / 2;
                    double y = (double)(h + (h - cel_size)) / 2;
                    pois.push_back(PointOfInterest(x, y));
                }
            }
        } else if (type == "P") {
            int num_pois; file >> num_pois;

            for (int p = 0; p < num_pois; p++) {
                double x, y; file >> x >> y;

                if (x < 0 || x > W || y < 0 || y > H) {
                    throw std::runtime_error("Invalid coordinate value");
                }

                pois.push_back(PointOfInterest(x, y));
            }
        }

        return pois;
    }
}