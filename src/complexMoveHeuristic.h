// Copyright 2014 Robert Horvath, Johannes Vogel
#ifndef SRC_COMPLEXMOVEHEURISTIC_H_
#define SRC_COMPLEXMOVEHEURISTIC_H_
#include <ctime>
#include "./instance.h"
#include "./solution.h"

/**
 * Applies different heuristics, appending several moves to 
 * create complex moves.
 * Stops trying to find further solutions if stop_time has been reached.
 */
bool complexMove(Solution* solution, time_t stop_time, bool best_improvement);

#endif  // SRC_COMPLEXMOVEHEURISTIC_H_
