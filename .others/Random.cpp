#include "Random.hpp"

Random::Random() {
    this->rng = std::mt19937(std::random_device{}());
    this->dist_0_1 = std::uniform_real_distribution<>(0, 1);
}

double Random::random() {
    return this->dist_0_1(this->rng);
}

double Random::random_value(double min, double max) {
    return min + (max - min) * this->random();
}

int Random::random_value(int min, int max) {
    return round(min + (max - min) * this->random());
}
