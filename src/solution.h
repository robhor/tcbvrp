// Copyright 2014 Robert Horvath, Johannes Vogel
#ifndef SRC_SOLUTION_H_
#define SRC_SOLUTION_H_
#include <vector>
#include "./instance.h"

using std::vector;

/// A tour is nothing but a list of nodes, visited in a certain order
typedef vector<int> Tour;

/// Represents a solution for a TCBVRP instance
class Solution {
 public:
    Solution();

    Instance *instance;             /// The instance this solution belongs to
    int length;                     /// Current length of tour
    vector<Tour*> *tours;           /// Vector of tours

    int get_cost();                 /// Get the total cost of the solution
    int get_tour_cost(int index);   /// Get the cost of the tour at index
    int get_tour_cost(Tour* tour);  /// Get the cost of the given tour

    void print();                   /// Print solution to stdout
    Solution* clone();              /// Malloc a clone
};

#endif  // SRC_SOLUTION_H_
