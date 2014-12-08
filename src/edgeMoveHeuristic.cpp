// Copyright 2014 Robert Horvath, Johannes Vogel
#include "./edgeMoveHeuristic.h"
#include "./edgeMover.h"

bool edgeMove(Solution* solution) {
    return edgeMove(solution, solution->length);
}

bool edgeMove(Solution* solution, int current_length) {
    EdgeMover em = EdgeMover(solution);

    while (em.next() != nullptr) {
        if (solution->length < current_length) {
            return true;
        }
    }

    return false;
}
