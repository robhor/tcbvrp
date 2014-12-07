// Copyright 2014 Robert Horvath, Johannes Vogel
#ifndef SRC_RANDOMGREEDY_H_
#define SRC_RANDOMGREEDY_H_
#include "./instance.h"
#include "./solution.h"

/// A greedy construction heuristic for the given TCBVRP
Solution* randomGreedy(Instance* instance, float alpha);

#endif  // SRC_RANDOMGREEDY_H_
