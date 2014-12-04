// Copyright 2014 Robert Horvath, Johannes Vogel
#include "./edgeMoveHeuristic.h"
#include "./edgeMover.h"

bool edgeMove(Solution* solution) {
    int current_length = solution->length;
    EdgeMover em = EdgeMover(solution);

    while (em.next() != nullptr) {
        if (solution->length < current_length) {
            return true;
        }
    }

    return false;
}
