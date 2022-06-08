#include <iostream>
#include <vector>
#include <string.h>

#include "HarmonySearch.hpp"
#include "HarmonySearchConfig.hpp"
#include "PointOfInterest.hpp"
#include "Sensor.hpp"

int main(int argc, char* argv[]) {
    HarmonySearchConfig config = HarmonySearchConfig();
    double radius, radius_err;

    std::cin >> config.W >> config.H >> config.hm_size;

    std::cin >> radius; radius_err = radius * 0.5;

    std::cin >> config.num_iterations;

    Sensor::set_values(radius, radius_err, config.W, config.H);

    if (PointOfInterest::read_pois(config.pois, config.W, config.H) == -1) {
        throw std::runtime_error("Invalid input");
    }

    // for (PointOfInterest poi : config.pois) {
    //     std::cout << "(" << poi.x << ", " << poi.y << ") ";
    // }
    // std::cout << std::endl;

    HarmonySearch hs = HarmonySearch(config);
    
    hs.starts();
    // hs.cout_harmony_memory();

    return 0;
}

