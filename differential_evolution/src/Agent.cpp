#include "Agent.hpp"
#include "Constants.hpp"

Agent::Agent() { }

Agent::Agent(std::vector<Sensor> sensors, std::vector<bool> active_sensors) {
    this->sensors = sensors;
    this->active_sensors = active_sensors;
    this->is_dominant = false;
}

void Agent::set_num_active_sensors(int num_active_sensors) {
    this->num_active_sensors = num_active_sensors;
}

void Agent::set_domination(bool is_dominant) {
    this->is_dominant = is_dominant;
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
