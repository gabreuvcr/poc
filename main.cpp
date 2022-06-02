#include <iostream>
#include <vector>
#include <string.h>

#include "HarmonySearch.hpp"
#include "PointOfInterest.hpp"

int main(int argc, char* argv[]) {
    int W, H, HM_size;
    double sensor_radius, sensor_uncertainty_radius;
    std::string type;

    std::cin >> W >> H >> HM_size;
    std::cin >> sensor_radius >> sensor_uncertainty_radius;

    std::vector<PointOfInterest> POIs;

    if (PointOfInterest::read_pois(POIs, W, H) == -1) {
        std::cout << "Invalid input" << std::endl;
    }

    for (PointOfInterest poi : POIs) {
        std::cout << "(" << poi.x << ", " << poi.y << ") ";
    }
    std::cout << std::endl;
    
    HarmonySearch hs = HarmonySearch(W, H, HM_size, sensor_radius, sensor_uncertainty_radius, POIs);
    
    hs.init_harmony_memory();
    hs.cout_harmony_memory();

    return 0;
}
