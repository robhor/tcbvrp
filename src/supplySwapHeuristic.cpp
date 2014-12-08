// Copyright 2014 Robert Horvath, Johannes Vogel
#include <iostream>
#include <string>

#include "supplySwapHeuristic.h"
#include "./supplySwapper.h"


bool supplySwap(Solution* solution) {
    int current_length = solution->length;
    SupplySwapper ss(solution);

    while (ss.next() != nullptr) {
        if (solution->length < current_length) {
            return true;
        }
    }
    
    return false;
}
