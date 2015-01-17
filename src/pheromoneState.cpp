// Copyright 2015 Robert Horvath, Johannes Vogel
#include <stdlib.h>
#include <math.h>
#include "./pheromoneState.h"

PheromoneState::PheromoneState(Instance* instance) {
    this->instance = instance;
    int nodes = instance->num_nodes + 1;
    pheromoneLevels = reinterpret_cast<double*>(
        malloc(sizeof(double) * nodes * nodes));

    for (int i = 0; i < nodes * nodes; i++) {
        pheromoneLevels[i] = 0;
    }
}

Instance* PheromoneState::get_instance() {
    return instance;
}

double PheromoneState::get_pheromone_level(int from, int to) {
    return pheromoneLevels[from * (instance->num_nodes+1) + to];
}

void PheromoneState::increase_pheromone_level(int from, int to, double amount) {
    pheromoneLevels[from * (instance->num_nodes+1) + to] += amount;
}

void PheromoneState::evaporate(double factor) {
    int nodes = instance->num_nodes + 1;
    for (int i = 0; i < nodes * nodes; i++) {
        pheromoneLevels[i] *= factor;
    }
}
void PheromoneState::evaporate2(double factor){
    int nodes = instance->num_nodes + 1;
    for (int i = 0; i < nodes * nodes; i++) {
        double level = pheromoneLevels[i];
        level*=factor;
        level += level*pow(factor,3.5);
        pheromoneLevels[i]=level;
    }
}
