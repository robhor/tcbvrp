// Copyright 2015 Robert Horvath, Johannes Vogel
#ifndef SRC_ANTCOLONY_H_
#define SRC_ANTCOLONY_H_
#include <vector>
#include "./solution.h"
#include "./pheromoneState.h"

class AntColony {
    Instance* instance;
    PheromoneState* pheromone_state;
    vector<Solution*> ant_solutions;
    void run_ants();
    void daemon_actions();
    void update_pheromones();

 public:
    explicit AntColony(Instance* instance);
    Solution* run();
    int num_ants;
};

#endif  // SRC_ANTCOLONY_H_
