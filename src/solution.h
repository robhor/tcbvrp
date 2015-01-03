// Copyright 2014 Robert Horvath, Johannes Vogel
#ifndef SRC_SOLUTION_H_
#define SRC_SOLUTION_H_
#include <stdio.h>
#include <vector>
#include "./instance.h"

using std::vector;

/// Represents a solution for a TCBVRP instance
class Solution {
 public:
    Solution();
    explicit Solution(Instance* instance);
    ~Solution();

    Instance *instance;    /// The instance this solution belongs to
    int length;            /// Current length of tour
    vector<Tour*> *tours;  /// Vector of tours

    int get_cost();                 /// Get the total cost of the solution
    int get_tour_cost(int index);   /// Get the cost of the tour at index
    int get_tour_cost(Tour* tour);  /// Get the cost of the given tour

    void print();                   /// Print solution to stdout
    void print(FILE* fd);           /// Print solution to a file descriptor
    Solution* clone();              /// Malloc a clone
    void set(Solution* solution);   /// Set this solution to be a copy of another
    void trim();                    /// Removes empty tours

    // Helpful methods

    /// Returns the node at the given index.
    /// The index iterates over all tours
    int node_at(int index);

    /// Returns the tour of the node at the given index
    Tour* tour_at(int index);

    /// Sets the node at the given index
    void set_node_at(int index, int node);

    /// Returns the predecessor of the node at the given index
    int predecessor_at(int index);

    /// Returns the successor of the node at the given index
    int successor_at(int index);

    /// Like set_node_at, but calculates length difference
    void replace_node_at(int index, int node);

    /// Insert node into tour at given index, calculates length difference
    void insert_node(Tour* tour, int index, int node);

    /// Remove node from tour at given index, calculates length difference
    void remove_node(Tour* tour, int index);
};

#endif  // SRC_SOLUTION_H_
