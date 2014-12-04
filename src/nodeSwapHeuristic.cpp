// Copyright 2014 Robert Horvath, Johannes Vogel
#include "nodeSwapHeuristic.h"


Solution* nodeSwap(Solution* solution) {
    int best_length = solution->length;
    NodeSwapper ns(solution);
    while (true) {
        while ((solution = ns.next()) != NULL) {
            if (solution->length < best_length) {
                best_length = solution->length;
                break;
            }
        }
        if (solution != NULL) {
            ns.accept();
        } else {
            solution = ns.reset();
            break;
        }
    }

    return solution;
}
