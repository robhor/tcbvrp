// Copyright 2014 Robert Horvath, Johannes Vogel
#ifndef SRC_TOURSPLITHEURISTIC_H_
#define SRC_TOURSPLITHEURISTIC_H_
#include "./instance.h"
#include "./solution.h"

/**
 * Applies the tour split heuristic to the solution.
 * Returns true if the solution has been improved, false otherwise.
 */
bool tourSplit(Solution* solution);

#endif  // SRC_TOURSPLITHEURISTIC_H_
