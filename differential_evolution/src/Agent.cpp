#include "Agent.hpp"
#include "Constants.hpp"

Agent::Agent() {
    this->num_active_sensors = 0;
    this->coverage_ratio = 0;
}

Agent::Agent(std::vector<Sensor> sensors, std::vector<bool> active_sensors) {
    this->sensors = sensors;
    this->active_sensors = active_sensors;
    this->calculate_num_active_sensors();
}

void Agent::set_num_active_sensors(int num_active_sensors) {
    this->num_active_sensors = num_active_sensors;
}

void Agent::calculate_num_active_sensors() {
    this->num_active_sensors = 0;
    for (int i = 0; i < active_sensors.size(); i++) {
        if (active_sensors[i]) {
            this->num_active_sensors++;
        }
    }
}

void Agent::calculate_coverage_ratio(std::vector<PointOfInterest> &pois) {
    int num_covered_pois = 0;
    for (PointOfInterest poi : pois) {
        if (poi.joint_coverage(sensors, active_sensors) > Constants::COVERAGE_THRESHOLD) {
            num_covered_pois++;
        }
    }
    this->coverage_ratio = (double)num_covered_pois / pois.size();
}

bool Agent::is_weak_dominat(Agent &agent) {
    if (this->num_active_sensors <= agent.num_active_sensors &&
        this->coverage_ratio >= agent.coverage_ratio) {
        return true;
    }
    
    return false;
}

bool Agent::is_dominant(Agent &agent) {
    if (this->is_weak_dominat(agent) &&
        (this->num_active_sensors < agent.num_active_sensors ||
        this->coverage_ratio > agent.coverage_ratio)) {
        
        return true;
    }

    return false;
}

bool Agent::operator ==(const Agent& other) {
    return this->num_active_sensors == other.num_active_sensors
        && this->coverage_ratio == other.coverage_ratio;
}
