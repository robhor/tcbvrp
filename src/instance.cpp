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
    distances[from * (num_nodes+1) + to] = distance;
}

int Instance::get_distance(int from, int to) {
    return distances[from * (num_nodes+1) + to];
}

void Instance::print_summary() {
    fprintf(stderr, "Nodes:          %i\n", num_nodes);
    fprintf(stderr, "Supply Nodes:   %lu\n", supply_nodes.size());
    fprintf(stderr, "Demand Nodes:   %lu\n", demand_nodes.size());
    fprintf(stderr, "Supply surplus: %lu\n", supply_nodes.size() - demand_nodes.size());
    fprintf(stderr, "Time Limit:     %i\n", time_limit);
    fprintf(stderr, "Vehicles:       %i\n", num_vehicles);
    fprintf(stderr, "\n");
}

int Instance::get_tour_cost(Tour *tour) {
    int cost = 0;
    int tour_size = tour->size();
    if (tour_size == 0) return cost;

    cost += get_distance(0, tour->at(0));
    for (int i = 0; i < tour_size - 1; i++) {
        int from = tour->at(i);
        int to   = tour->at(i+1);
        int distance = get_distance(from, to);
        cost += distance;
    }
    cost += get_distance(tour->at(tour_size-1), 0);
    return cost;
}


Instance::~Instance() {
    if (distances != NULL) free(distances);
}



Instance* ReadInstanceFile(const char* path) {
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
        int to   = i % (num_nodes + 1);
        instance->set_distance(from, to, distance);
        i++;
    }

    return instance;
}
