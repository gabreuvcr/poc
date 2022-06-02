#include <iostream>
#include <vector>
#include <string.h>

#include "HarmonySearch.hpp"
#include "PointOfInterest.hpp"

int main(int argc, char* argv[]) {
    int W = 50;
    int H = 50;
    int HM_size = 5;
    double sensor_radius = 10;
    double sensor_uncertainty_radius = 5;

    std::vector<PointOfInterest> pois;

    if (argc == 3 && strcmp(argv[1], "-g") == 0) {
        int cel_size = atoi(argv[2]);

        if (W % cel_size != 0 || H % cel_size != 0 || cel_size < 1 || cel_size > W || cel_size > H) {
            std::cout << "Invalid <cel_size> value" << std::endl;
            return 1;
        }

        pois = PointOfInterest::generate_pois_from_grid(W, H, cel_size);

        std::cout << "Generated " << pois.size() << " POIs" << std::endl;
        for (PointOfInterest poi : pois) {
            std::cout << "(" << poi.x << ", " << poi.y << ") ";
        }
        std::cout << std::endl;

    } else {
        std::cout << "Usage: " << argv[0] << " -g <cel_size>" << std::endl;
        std::cout << "Wrong arguments" << std::endl;
        return 1;
    }

    HarmonySearch hs(W, H, HM_size, sensor_radius, sensor_uncertainty_radius);
    
    hs.init_harmony_memory();
    hs.cout_harmony_memory();
    
    return 0;
}
