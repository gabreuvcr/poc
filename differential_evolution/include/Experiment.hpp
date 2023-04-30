#ifndef EXPERIMENT_HPP
#define EXPERIMENT_HPP

#include "DifferentialEvolution.hpp"
#include "PointOfInterest.hpp"

namespace Experiment {
    void run_test(DifferentialEvolutionConfig config, std::vector<PointOfInterest> pois);
}

#endif