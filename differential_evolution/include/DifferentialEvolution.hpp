#ifndef DIFFERENTIAL_EVOLUTION_H
#define DIFFERENTIAL_EVOLUTION_H

#include <iostream>
#include <vector>

#include "Sensor.hpp"
#include "PointOfInterest.hpp"

class DifferentialEvolution {
    private:
        std::vector<std::vector<Sensor>> population;
        void init_first_population();
        std::vector<Sensor> mutation(std::vector<Sensor> indivi);
        std::vector<Sensor> crossover(std::vector<Sensor> agent_a, std::vector<Sensor> agent_b);
        void selection();

    public:
        std::vector<PointOfInterest> pois;
        
        DifferentialEvolution();
        
        double run();
};

#endif