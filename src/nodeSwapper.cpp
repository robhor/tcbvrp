// Copyright 2014 Robert Horvath, Johannes Vogel
#include "./nodeSwapper.h"
#include <stdio.h>

NodeSwapper::NodeSwapper(Solution* solution) {
    currentSolution = solution;
    i = 0;
    j = 1;
}

Solution* NodeSwapper::next() {
    // swap node i <-> node j from original solution
    return NULL;
}
