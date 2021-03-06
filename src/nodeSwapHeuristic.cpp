// Copyright 2014 Robert Horvath, Johannes Vogel
#include "nodeSwapHeuristic.h"
#include "./nodeSwapper.h"

bool nodeSwap(Solution* solution) {
    int current_length = solution->length;
    NodeSwapper ns(solution);

    while (ns.next() != nullptr) {
        if (solution->length < current_length) {
            return true;
        }
    }
    solution = ns.reset();
    return false;
}
