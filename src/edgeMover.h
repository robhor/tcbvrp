// Copyright 2014 Robert Horvath, Johannes Vogel
#ifndef SRC_EDGEMOVER_H_
#define SRC_EDGEMOVER_H_
#include "./instance.h"
#include "./solution.h"

class EdgeMover {
    Solution* solution;   /// The solution to manipulate
    int src_tour, src;    /// Indices of the source of the edge move
    int dst_tour, dst;    /// Indices of the destination of the edge move
    bool increment();     /// Move on to the next indices

    /// Perform edge move
    void move_edge(int src_tour, int src, int dst_tour, int dst);
    bool move_valid();    /// Returns whether or not last edge move was valid

 public:
    explicit EdgeMover(Solution* solution);
    ~EdgeMover();

    Solution* next();    /// Generate next solution
    void accept();       /// Accept the last generated solution as new base
    Solution* reset();   /// Undo last edge move
};

#endif  // SRC_EDGEMOVER_H_
