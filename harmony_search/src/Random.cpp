#include "Random.hpp"
#include "Constants.hpp"

namespace Random {
    double random() {
        return dist_0_1(rng);
    }

    double random_value(double min, double max) {
        return min + (max - min) * random();
    }

    int random_value(int min, int max) {
        return floor(min + (max - min + 1) * random());
    }

    std::mt19937 rng = std::mt19937(Constants::SEED);
    std::uniform_real_distribution<> dist_0_1 = std::uniform_real_distribution<>(0, 1);
}
