// Copyright 2014 Robert Horvath, Johannes Vogel
#ifndef SRC_INSTANCE_H_
#define SRC_INSTANCE_H_
#include <vector>

/**
 * Encapsulates information about a problem instance.
 * Immutable.
 */
class Instance {
    int *distances;

 public:
    int num_nodes;                  /// The number of nodes
    int num_vehicles;               /// The number of vehicles
    int time_limit;                 /// The time limit per vehicle
    std::vector<int> supply_nodes;  /// A vector of supply nodes
    std::vector<int> demand_nodes;  /// A vector of demand nodes

    ~Instance();
    void init();                    /// Allocates memory for distances matrix
    void set_distance(int from, int to, int distance);
    int get_distance(int from, int to);
    void print_summary();
};


/**
 * Read an instance file and return a parsed Instance object, 
 * or null in case of an error
 */
Instance* ReadInstanceFile(char* path);

#endif  // SRC_INSTANCE_H_
