// Copyright 2014 Robert Horvath, Johannes Vogel
#ifndef SRC_TOURSPLITTER_H_
#define SRC_TOURSPLITTER_H_
#include "./instance.h"
#include "./solution.h"

class TourSplitter {
    Solution* solution;  /// The solution to manipulate
    Tour* buffer_tour;   /// Where the split off tour is moved to
    bool increment();    /// Move on to the next possible split
    int split_begin;     /// Index of the start of the subtour to split off
    int split_end;       /// Index of the end of the subtour to split off
    void split();        /// Perform split


 public:
    explicit TourSplitter(Solution* solution);
    ~TourSplitter();

    bool next();         /// Generate next solution
    Solution* reset();   /// Undo last edge move
};

#endif  // SRC_TOURSPLITTER_H_
