// Copyright 2014 Robert Horvath, Johannes Vogel
#ifndef SRC_NODESWAPPER_H_
#define SRC_NODESWAPPER_H_
#include "./instance.h"
#include "./solution.h"

class NodeSwapper {
    Solution* currentSolution;
    int i, j, num_nodes;
    bool swapped;

    bool increment();   /// Increment i and j, return false no more indices
    void swap_nodes();  /// Swap current nodes i and j
    bool swap_valid();  /// Checks if last swap violates time limits


 public:
    explicit NodeSwapper(Solution* solution);
    Solution* next();   /// Generates the next swapped solution
    void accept();      /// Accepts the last generated solution as a new base
    Solution* reset();  /// Undo swap, retrieve original solution
};

#endif  // SRC_NODESWAPPER_H_

