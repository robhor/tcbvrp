// Copyright 2015 Robert Horvath, Johannes Vogel
#include "./ant.h"
#include <stdlib.h>
#include <math.h>
#include <ctime>
#include <vector>
#include <set>

using std::set;
using std::vector;

int nearest_node(int node, set<int> candidates, Instance* instance);

static unsigned int seedp = std::time(nullptr);

Ant::Ant(PheromoneState* pheromone_state) {
    this->pheromone_state = pheromone_state;
    vector<int> supply_nodes = pheromone_state->get_instance()->supply_nodes;
    vector<int> demand_nodes = pheromone_state->get_instance()->demand_nodes;

    unused_supplies.insert(supply_nodes.begin(), supply_nodes.end());
    unused_demands.insert(demand_nodes.begin(), demand_nodes.end());
}

Solution* Ant::run() {
    Solution* solution = new Solution(pheromone_state->get_instance());
    tour.clear();

    while (unused_demands.size() > 0) {
        int supply = next_supply();

        if (supply != 0) {
            tour.push_back(supply);
            unused_supplies.erase(supply);

            int demand = next_demand();
            tour.push_back(demand);
            unused_demands.erase(demand);
        } else {
            solution->tours->push_back(new Tour(tour));
            solution->length += solution->get_tour_cost(&tour);
            tour.clear();
        }
    }

    if (tour.size() > 0) {
        solution->tours->push_back(new Tour(tour));
        solution->length += solution->get_tour_cost(&tour);
        tour.clear();
    }

    return solution;
}

/// Get the next supply node on the ant's walk
int Ant::next_supply() {
    int current_node;
    
    if (tour.size() == 0) {
        current_node = 0;
    } else {
        current_node = tour.back();
    }

    // candidate nodes:
    // supply nodes, where closest demand node + back to depot
    // is still possible within time constraint

    Instance *instance = pheromone_state->get_instance();
    int tour_length = instance->get_tour_cost(&tour);

    vector<int> candidates;
    for (auto next_supply : unused_supplies) {
        for (auto next_demand : unused_demands) {
            int dist = tour_length;
            dist += instance->get_distance(current_node, next_supply);
            dist += instance->get_distance(next_supply, next_demand);
            dist += instance->get_distance(next_demand, 0);

            if (dist <= instance->time_limit) {
                candidates.push_back(next_supply);
            }
        }
    }

    if (candidates.size() == 0) return 0;
    return select_node(current_node, candidates);
}

int Ant::next_demand() {
    // calculate probabilities for each of those and then select one randomly
    int current_node = tour.back();
    vector<int> candidates =
        vector<int>(unused_demands.begin(), unused_demands.end());
    return select_node(current_node, candidates);
}

int Ant::select_node(int current_node, vector<int> candidates) {
    Instance *inst = pheromone_state->get_instance();
    vector<double> probabilities;
    double probabilities_sum = 0;
    int medium_probability = 0;
    for (auto node : candidates) {
        double pheromone_level =
            pheromone_state->get_pheromone_level(current_node, node) + 1;

        double visibility =
            1.0f / static_cast<double>(inst->get_distance(current_node, node));

        double probability =
            pow(pheromone_level, alpha) * pow(visibility, beta);

        probabilities.push_back(probability);
        probabilities_sum += probability;
    }
    medium_probability = probabilities_sum/candidates.size();

    double random_number = static_cast<double>(rand_r(&seedp)) / RAND_MAX;
    double p = 0;
    for (size_t i = 0; i < candidates.size(); i++) {
        int node = candidates.at(i);

        double probability = probabilities.at(i);
        if (probabilities_sum > 0) {
            probability /= probabilities_sum;
        } else {
            probability = 1.0f / candidates.size();
        }




        //set probability to 0.005 if it is to low
        if(probability<0.2*medium_probability&&medium_probability>0.015){
            probability = 0.0005;
        }


        p += probability;
        if (random_number <= p) {
            return node;
        }
    }

    return 0;
}


int nearest_node(int node, set<int> candidates, Instance* instance) {
    if (candidates.size() == 0) return -1;

    int min_distance = instance->get_distance(node, *(candidates.begin()));
    int nearest_node = *(candidates.begin());

    for (auto current_node : candidates) {
        int distance = instance->get_distance(node, current_node);
        if (distance < min_distance) {
            min_distance = distance;
            nearest_node = current_node;
        }
    }

    return nearest_node;
}
