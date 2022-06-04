#include <iostream>
#include <vector>
#include <string.h>

#include "HarmonySearch.hpp"
#include "HarmonySearchConfig.hpp"
#include "PointOfInterest.hpp"
#include "Sensor.hpp"

int main(int argc, char* argv[]) {
    HarmonySearchConfig config = HarmonySearchConfig();

    std::cin >> config.W >> config.H >> config.hm_size;
    std::cin >> config.sensor_radius >> config.sensor_uncertainty_radius;

    if (PointOfInterest::read_pois(config.pois, config.W, config.H) == -1) {
        throw std::runtime_error("Invalid input");
    }

    // for (PointOfInterest poi : config.pois) {
    //     std::cout << "(" << poi.x << ", " << poi.y << ") ";
    // }
    // std::cout << std::endl;

    Sensor::sensor_radius = config.sensor_radius;
    Sensor::sensor_uncertainty_radius = config.sensor_uncertainty_radius;

    HarmonySearch hs = HarmonySearch(config);
    
    hs.init_harmony_memory();
    hs.cout_harmony_memory();

    return 0;
}

