// Copyright 2014 Robert Horvath, Johannes Vogel
#include "./supplySwapper.h"
#include <stdio.h>
#include <vector>
#include <algorithm>

using std::vector;

SupplySwapper::SupplySwapper(Solution* solution) {
    old_node = -1;
    remaining_supplies = vector<int>(solution->instance->supply_nodes);
    this->solution = solution;

    num_nodes = solution->instance->demand_nodes.size() * 2;
    for (int i = 0; i < num_nodes; i += 2) {
        int node = solution->node_at(i);
        vector<int>::iterator it = find(remaining_supplies.begin(), remaining_supplies.end(), node);

       if (it != remaining_supplies.end()) {
            remaining_supplies.erase(it);
       }
    }

    i = -2;  // Iterates over the supply nodes in the solution
    j = 0;   // Iterates over the remaining supply nodes
}

Solution* SupplySwapper::reset() {
    if (old_node == -1) return solution;

    solution->replace_node_at(i, old_node);
    return solution;
}

/// Increment indices. Returns false if no more indices to consider
bool SupplySwapper::increment() {
    i += 2;
    if (i >= num_nodes) {
        i = 0;
        j++;
    }
    int remaining_size = remaining_supplies.size();
    return j < remaining_size;
}

void SupplySwapper::swap_nodes() {
    old_node = solution->node_at(i);
    int new_node = remaining_supplies.at(j);
    solution->replace_node_at(i, new_node);
}

bool SupplySwapper::swap_valid() {
    Tour* tour = solution->tour_at(i);
    int tour_length = solution->get_tour_cost(tour);

    return tour_length <= solution->instance->time_limit;
}

Solution* SupplySwapper::next() {
    while (true) {
        reset();
        if (!increment()) return nullptr;
        swap_nodes();
        if (swap_valid()) return solution;
    }
}
