#ifndef HARMONY_SEARCH_CONFIG_H
#define HARMONY_SEARCH_CONFIG_H

#include <vector>

#include "PointOfInterest.hpp"

class HarmonySearchConfig {
    public:
        std::vector<PointOfInterest> pois;
        int W, H, hm_size, num_iterations;

        HarmonySearchConfig() {}
};

#endif
