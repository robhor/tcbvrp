// Copyright 2014 Robert Horvath, Johannes Vogel
#ifndef SRC_SUPPLYSWAPHEURISTIC_H_
#define SRC_SUPPLYSWAPHEURISTIC_H_
#include "./instance.h"
#include "./solution.h"

/**
 * Performs supplynode swapping heuristic.
 * Returns true if solution has been improved, false otherwise.
 */
bool supplySwap(Solution* solution);

#endif  // SRC_SUPPLYSWAPHEURISTIC_H_
