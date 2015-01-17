// Copyright 2015 Robert Horvath, Johannes Vogel
#ifndef SRC_ANTCOLONY_H_
#define SRC_ANTCOLONY_H_
#include <vector>
#include <boost/thread/mutex.hpp>
#include "./solution.h"
#include "./pheromoneState.h"

class AntColony {
    Instance* instance;
    PheromoneState* pheromone_state;
    vector<Solution*> ant_solutions;
    boost::mutex mtx;
    void run_ants();
    void run_ant();
    void daemon_actions();
    void update_pheromones(int);
    void update_pheromones_1();
    void update_pheromones_2();

 public:
    explicit AntColony(Instance* instance);
    Solution* run();
    int num_ants;
    int pheromone_model;
};

#endif  // SRC_ANTCOLONY_H_
