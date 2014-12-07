// Copyright 2014 Robert Horvath, Johannes Vogel
#ifndef SRC_SUPPLYSWAPPER_H_
#define SRC_SUPPLYSWAPPER_H_
#include "./instance.h"
#include "./solution.h"

class SupplySwapper {
    Solution* solution;
    int i, j, num_nodes;
    vector<int> remaining_supplies;
    int old_node;

    bool increment();   /// Increment i and j, return false no more indices
    void swap_nodes();  /// Swap current nodes i and j
    bool swap_valid();  /// Checks if last swap violates time limits

 public:
    explicit SupplySwapper(Solution* solution);
    Solution* next();   /// Generates the next swapped solution
    void accept();      /// Accepts the last generated solution as a new base
    Solution* reset();  /// Undo swap, retrieve original solution
};

#endif  // SRC_SUPPLYSWAPPER_H_

