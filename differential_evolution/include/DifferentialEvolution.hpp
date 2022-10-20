#ifndef DIFFERENTIAL_EVOLUTION_H
#define DIFFERENTIAL_EVOLUTION_H

#include <iostream>
#include <vector>
#include "DifferentialEvolutionConfig.hpp"
#include "Sensor.hpp"
#include "PointOfInterest.hpp"
#include "Agent.hpp"

class DifferentialEvolution {
    private:
        void init_first_population();
        std::vector<Sensor> mutation_sensors(int i);
        std::vector<Sensor> crossover_sensors(int i, std::vector<Sensor> mutated_sensors);
        // void selection();

        void update_dominat_sensors();

    public:
        DifferentialEvolutionConfig config;
        std::vector<PointOfInterest> pois;
        std::vector<Agent> population;
        int max_sensores, min_sensores;
        
        DifferentialEvolution(DifferentialEvolutionConfig config, std::vector<PointOfInterest> pois);
        
        void run();
        void run2();
};

#endif