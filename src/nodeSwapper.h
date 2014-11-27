// Copyright 2014 Robert Horvath, Johannes Vogel
#ifndef SRC_NODESWAPPER_H_
#define SRC_NODESWAPPER_H_
#include "./instance.h"
#include "./solution.h"

class NodeSwapper {
    Solution* currentSolution;
    int i, j;

 public:
    explicit NodeSwapper(Solution* solution);
    Solution* next();
};

#endif  // SRC_NODESWAPPER_H_

