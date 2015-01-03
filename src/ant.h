// Copyright 2015 Robert Horvath, Johannes Vogel
#ifndef SRC_ANT_H_
#define SRC_ANT_H_
#include <set>
#include <vector>
#include "./solution.h"
#include "./pheromoneState.h"

using std::set;
using std::vector;

class Ant {
    PheromoneState* pheromone_state;
    set<int> unused_supplies;
    set<int> unused_demands;

    int next_supply();
    int next_demand();

    /// Randomly selects a node, based on pheromones and visibility
    int select_node(int current_node, vector<int> candidates);

    Tour tour;

 public:
    explicit Ant(PheromoneState* pheromoneState);
    double alpha = 1;
    double beta  = 1;

    Solution* run();
};

#endif  // SRC_ANT_H_
