// Copyright 2014 Robert Horvath, Johannes Vogel
#include "./instance.h"
#include <stdio.h>
#include <fstream>

using std::ifstream;

void Instance::init() {
    int nodes = num_nodes + 1;  // including special node 0
    distances = reinterpret_cast<int*>(malloc(sizeof(int) * nodes * nodes));
}

void Instance::set_distance(int from, int to, int distance) {
    distances[from * num_nodes + to] = distance;
}

int Instance::get_distance(int from, int to) {
    return distances[from * num_nodes + to];
}

void Instance::print_summary() {
    printf("Nodes:      %i\n", num_nodes);
    printf("Time Limit: %i\n", time_limit);
    printf("Vehicles:   %i\n", num_vehicles);
}

Instance::~Instance() {
    if (distances != NULL) free(distances);
}



Instance* ReadInstanceFile(char* path) {
    ifstream infile(path);

    Instance *instance = new Instance;

    int num_nodes;
    infile >> num_nodes;
    instance->num_nodes = num_nodes;
    infile >> instance->time_limit;
    infile >> instance->num_vehicles;
    instance->init();

    // Read nodes
    int node, i = 0;
    char type;
    while (i < num_nodes) {
        infile >> node >> type;
        if (type == 'S') {
            instance->supply_nodes.push_back(node);
        } else {
            instance->demand_nodes.push_back(node);
        }
        i++;
    }

    // Read distance matrix
    int distance;
    i = 0;
    while (infile >> distance) {
        int from = i / (num_nodes + 1);
        int to = i % (num_nodes + 1);
        instance->set_distance(from, to, distance);
        i++;
    }

    return instance;
}
