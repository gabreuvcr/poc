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
        std::vector<Sensor> crossover_sensors(std::vector<Sensor> agent_sensors, std::vector<Sensor> mutated_sensors);
        
        Agent tournament_selection(int i);
        std::vector<bool> crossover_active_sensors(std::vector<bool> agent_active_sensors, std::vector<bool> win_active_sensors);
        std::vector<bool> mutation_active_sensors(std::vector<bool> crossed_active_sensors);
        std::vector<Agent> calculate_crowding_distance(std::vector<Agent>& pop);

    public:
        DifferentialEvolutionConfig config;
        std::vector<PointOfInterest> pois;
        std::vector<Agent> population;
        int max_sensores, min_sensores;
        
        DifferentialEvolution(DifferentialEvolutionConfig config, std::vector<PointOfInterest> pois);
        
        std::vector<Agent> run();
        std::vector<Agent> get_pareto_front();
        std::vector<Agent> get_pareto_front(std::vector<Agent> &population);
};

#endif