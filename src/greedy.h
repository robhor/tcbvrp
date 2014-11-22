// Copyright 2014 Robert Horvath, Johannes Vogel
#ifndef SRC_GREEDY_H_
#define SRC_GREEDY_H_
#include "./instance.h"
#include "./solution.h"

/// A greedy construction heuristic for the given TCBVRP
Solution* greedy(Instance* instance);

#endif  // SRC_GREEDY_H_
