#ifndef AGENT_H
#define AGENT_H

#include <vector>
#include "Sensor.hpp"
#include "PointOfInterest.hpp"

class Agent {
    public:
        std::vector<Sensor> sensors;
        std::vector<bool> active_sensors;
        bool is_dominant;

        int num_active_sensors;
        double coverage_ratio;
        
        Agent();
        Agent(std::vector<Sensor> sensors, std::vector<bool> active_sensors);
        Agent(std::vector<Sensor> sensors, std::vector<bool> active_sensors, bool fitness);

        void set_num_active_sensors(int num_active_sensors);
        void set_domination(bool is_dominant);
        void calculate_coverage_ratio(std::vector<PointOfInterest> &pois);
        bool check_dominance(std::vector<PointOfInterest> &pois);
};

#endif