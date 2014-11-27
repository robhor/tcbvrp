// Copyright 2014 Robert Horvath, Johannes Vogel
#include "./nodeSwapper.h"
#include <stdio.h>

int get_nth_node(Solution* solution, int n) {
    for (auto tour : *solution->tours) {
        int tour_size = tour->size();
        if (n < tour_size) {
            return tour->at(n);
        } else {
            n -= tour_size;
        }
    }
    return -1;
}

NodeSwapper::NodeSwapper(Solution* solution) {
    currentSolution = solution;
    this->solution = solution->clone();
    i = 0;
    j = 0;
}

Solution* NodeSwapper::next() {
    // increment
    j = (j+1) % (currentSolution->instance->num_nodes);
    if (j == 0) i++;

    if (i >= currentSolution->instance->num_nodes) {
        // we're done here, checked all nodes
        return NULL;
    }

    // swap node i <-> node j from original solution
    int node_i = get_nth_node(currentSolution, i);
    int node_j = get_nth_node(currentSolution, j);

    // swap them
    solution = currentSolution->clone();  // clone current solution
    // TODO(robhor) swap nodes
    // TODO(robhor) recalculate length

    return solution;
}
