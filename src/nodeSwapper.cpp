// Copyright 2014 Robert Horvath, Johannes Vogel
#include "./nodeSwapper.h"
#include <stdio.h>

void swap_nodes(Solution* solution, int i, int j) {
    int node_i = solution->node_at(i);
    int node_j = solution->node_at(j);

    solution->replace_node_at(i, node_j);
    solution->replace_node_at(j, node_i);
}

NodeSwapper::NodeSwapper(Solution* solution) {
    currentSolution = solution;
    num_nodes = currentSolution->instance->demand_nodes.size() * 2;
    i = 0;
    j = 0;
}

void NodeSwapper::accept() {
    i = 0;
    j = 0;
}

Solution* NodeSwapper::reset() {
    if (!(i == 0 && j == 0) && j < num_nodes) {
        // currently swapped, swap back
        swap_nodes(currentSolution, i, j);
    }
    return currentSolution;
}

Solution* NodeSwapper::next() {
    reset();

    // increment
    j = j+2;
    if (j >= num_nodes) {
        i++;
        j = i + 2;
    }

    if (j >= num_nodes) {
        // we're done here, checked all nodes
        return NULL;
    }

    swap_nodes(currentSolution, i, j);

    return currentSolution;
}
