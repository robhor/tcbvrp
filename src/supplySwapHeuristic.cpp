// Copyright 2014 Robert Horvath, Johannes Vogel
#include <iostream>
#include <string>

#include "supplySwapHeuristic.h"
#include "./supplySwapper.h"


bool supplySwap(Solution* solution) {
    int current_length = solution->length;
    SupplySwapper ns(solution);
    std::string test;


    while (ns.next() != nullptr) {
        if (solution->length < current_length) {
        	solution->print();
            std::getline(std::cin,test);
            return true;
        }
    }
    solution = ns.reset();
    return false;
}
