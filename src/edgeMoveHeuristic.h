// Copyright 2014 Robert Horvath, Johannes Vogel
#ifndef SRC_EDGEMOVEHEURISTIC_H_
#define SRC_EDGEMOVEHEURISTIC_H_
#include "./instance.h"
#include "./solution.h"

/**
 * Applies the edge move heuristic to the solution.
 * Returns true if the solution has been improved, false otherwise.
 */
bool edgeMove(Solution* solution);

#endif  // SRC_EDGEMOVEHEURISTIC_H_
