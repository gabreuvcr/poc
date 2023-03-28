#include <fstream>
#include <string.h>
#include <vector>
#include <chrono>
#include "Utils.hpp"

namespace Utils {
    void log_population(std::vector<Agent> population) {
        for (int i = 0; i < population.size(); i++) {
            std::cout << population[i].num_active_sensors << "|";
            std::cout << population[i].coverage_ratio << ";";
            // if (i != population.size() - 1) {
            //     std::cout << ";";
            // }
        }
        std::cout << std::endl;
    }

    void log_time(std::chrono::duration<double> time) {
        std::cout << "Average time: " << time.count() << " s" << std::endl;
    }

    void log_seed(int seed) {
        std::cout << "Seed: " << seed << std::endl;
    }

    void invalid_argument() {
        throw std::runtime_error("Usage: ./main <input_file> [-all]");
        exit(1);
    }
    
    void check_arguments(char *argv[], int argc, std::string &filename, bool &all) {
        if (argc < 2) invalid_argument();

        if (argc >= 2) filename = argv[1];
        
        if (argc >= 3) {
            for (int i = 2; i < argc; i++) {
                if (all) invalid_argument();

                if (strcmp(argv[i], "-all") == 0) {
                    all = true;
                }
                else {
                    invalid_argument();
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

    DifferentialEvolutionConfig read_differential_evolution_config(std::ifstream &file) {
        DifferentialEvolutionConfig config = DifferentialEvolutionConfig();

        file >> config.W >> config.H;
        file >> config.pop_size >> config.num_generation;

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
