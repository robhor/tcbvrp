// Copyright 2014 Robert Horvath, Johannes Vogel
#include "./tourSplitHeuristic.h"
#include "./tourSplitter.h"

bool tourSplit(Solution* solution) {
    int current_length = solution->length;
    TourSplitter ts = TourSplitter(solution);

    while (ts.next()) {
        if (solution->length < current_length) {
            return true;
        }
    }

    ts.reset();
    return false;
}
