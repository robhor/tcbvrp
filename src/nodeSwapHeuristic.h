// Copyright 2014 Robert Horvath, Johannes Vogel
#ifndef SRC_NODESWAPHEURISTIC_H_
#define SRC_NODESWAPHEURISTIC_H_
#include "./instance.h"
#include "./solution.h"

/**
 * Performs node swapping heuristic.
 * Returns true if solution has been improved, false otherwise.
 */
bool nodeSwap(Solution* solution);

#endif  // SRC_NODESWAPHEURISTIC_H_
