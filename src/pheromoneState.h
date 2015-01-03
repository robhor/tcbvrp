// Copyright 2015 Robert Horvath, Johannes Vogel
#ifndef SRC_PHEROMONESTATE_H_
#define SRC_PHEROMONESTATE_H_
#include "./instance.h"

class PheromoneState {
    Instance* instance;
    double *pheromoneLevels;

 public:
    explicit PheromoneState(Instance* instance);
    Instance* get_instance();
    double get_pheromone_level(int from, int to);
    void increase_pheromone_level(int from, int to, double amount);
    void evaporate(double factor);
};

#endif  // SRC_PHEROMONESTATE_H_
