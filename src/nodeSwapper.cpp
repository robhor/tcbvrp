// Copyright 2014 Robert Horvath, Johannes Vogel
#include "./nodeSwapper.h"
#include <stdio.h>

NodeSwapper::NodeSwapper(Solution* solution) {
    currentSolution = solution;
    num_nodes = currentSolution->instance->demand_nodes.size() * 2;
    i = 0;
    j = 0;
    swapped = false;
}

void NodeSwapper::accept() {
    swapped = false;
    // i = 0;
    // j = 0;
}

Solution* NodeSwapper::reset() {
    if (swapped && j < num_nodes) {
        // currently swapped, swap back
        swap_nodes();
    }
    return currentSolution;
}

/// Increment indices. Returns false if no more indices to consider
bool NodeSwapper::increment() {
    j = j+2;
    if (j >= num_nodes) {
        i++;
        j = i + 2;
    }

    if (j >= num_nodes) {
        // we're done here, checked all nodes
        return false;
    }

    return true;
}

void NodeSwapper::swap_nodes() {
    swapped = !swapped;
    int node_i = currentSolution->node_at(i);
    int node_j = currentSolution->node_at(j);

    currentSolution->replace_node_at(i, node_j);
    currentSolution->replace_node_at(j, node_i);
}

bool NodeSwapper::swap_valid() {
    int length;
    Tour* a = currentSolution->tour_at(i);
    Tour* b = currentSolution->tour_at(j);

    length = currentSolution->get_tour_cost(a);
    if (length > currentSolution->instance->time_limit)
        return false;

    if (a == b) return true;

    length = currentSolution->get_tour_cost(b);
    if (length > currentSolution->instance->time_limit)
        return false;

    return true;
}

Solution* NodeSwapper::next() {
    while (true) {
        reset();
        if (!increment()) return NULL;
        swap_nodes();
        if (swap_valid()) return currentSolution;
    }
}
