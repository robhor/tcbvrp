// Copyright 2014 Robert Horvath, Johannes Vogel
#ifndef SRC_TOURSPLITTER_H_
#define SRC_TOURSPLITTER_H_
#include "./instance.h"
#include "./solution.h"

class TourSplitter {
    Solution* solution;  /// The solution to manipulate
    Tour* buffer_tour;   /// Where the split off tour is moved to

    bool increment();    /// Move on to the next possible split

 public:
    int split_index;     /// Indices of the source of the edge move
    void split();        /// Perform split
    explicit TourSplitter(Solution* solution);
    ~TourSplitter();

    bool next();         /// Generate next solution
    Solution* reset();   /// Undo last edge move
};

#endif  // SRC_TOURSPLITTER_H_
