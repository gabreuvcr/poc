#ifndef AGENT_H
#define AGENT_H

#include <vector>
#include "Sensor.hpp"
#include "PointOfInterest.hpp"

class Agent {
    public:
        std::vector<Sensor> sensors;
        std::vector<bool> active_sensors;

        int num_active_sensors;
        double coverage_ratio;
        double distance;
        
        Agent();
        Agent(std::vector<Sensor> sensors, std::vector<bool> active_sensors);

        void set_num_active_sensors(int num_active_sensors);
        void calculate_num_active_sensors();
        void calculate_coverage_ratio(std::vector<PointOfInterest> &pois);
        bool check_dominance(std::vector<PointOfInterest> &pois);
        bool is_dominant(Agent &agent);
        bool is_weak_dominat(Agent &agent);
        bool operator ==(const Agent& other);
};

#endif