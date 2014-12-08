// Copyright 2014 Robert Horvath, Johannes Vogel
#include "./complexMoveHeuristic.h"
#include "./nodeSwapper.h"
#include "./edgeMover.h"
#include "./supplySwapper.h"

bool complexMove(Solution* solution, time_t stop_time,
                 int moves, int current_length);
bool complexMove_nodeSwap(Solution* solution, time_t stop_time,
                          bool best_improvement, int moves, int current_length);
bool complexMove_edgeMove(Solution* solution, time_t stop_time,
                          bool best_improvement, int moves, int current_length);
bool complexMove_supplySwap(Solution* solution, time_t stop_time,
                          bool best_improvement, int moves, int current_length);


bool time_up(time_t stop_time) {
    time_t current_time = time(nullptr);
    if (current_time > stop_time) return true;
    return false;
}

bool complexMove(Solution* solution, time_t stop_time, bool best_improvement,
                 int moves, int current_length) {
    if (time_up(stop_time)) return false;
    return complexMove_nodeSwap(solution, stop_time, best_improvement, moves, current_length) ||
           complexMove_edgeMove(solution, stop_time, best_improvement, moves, current_length) ||
           complexMove_supplySwap(solution, stop_time, best_improvement, moves, current_length);
}


bool complexMove_nodeSwap(Solution* solution, time_t stop_time,
                          bool best_improvement, int moves, int current_length) {
    NodeSwapper ns(solution);

    if (best_improvement && moves == 1) {
        Solution *best_solution = solution->clone();
        while (ns.next()) {
            if (solution->length < best_solution->length) {
                delete best_solution;
                best_solution = solution->clone();
            }
        }

        if (best_solution->length < current_length) {
            solution->set(best_solution);
            return true;
        } else {
            return false;
        }
    }

    while (ns.next()) {
        if (solution->length < current_length) {
            return true;
        } else if (moves > 1) {
            if (complexMove(solution, stop_time, best_improvement, moves-1, current_length)) {
                return true;
            }
        }
    }
    return false;
}

bool complexMove_edgeMove(Solution* solution, time_t stop_time,
                          bool best_improvement, int moves, int current_length) {
    EdgeMover em(solution);

    if (best_improvement && moves == 1) {
        Solution *best_solution = solution->clone();
        while (em.next()) {
            if (solution->length < best_solution->length) {
                delete best_solution;
                best_solution = solution->clone();
            }
        }

        if (best_solution->length < current_length) {
            solution->set(best_solution);
            return true;
        } else {
            return false;
        }
    }

    while (em.next()) {
        if (solution->length < current_length) {
            return true;
        } else if (moves > 1) {
            if (complexMove(solution, stop_time, best_improvement, moves-1, current_length)) {
                return true;
            }
        }
    }
    return false;
}

bool complexMove_supplySwap(Solution* solution, time_t stop_time,
                           bool best_improvement, int moves, int current_length) {
    SupplySwapper ss(solution);

    if (best_improvement && moves == 1) {
        Solution *best_solution = solution->clone();
        while (ss.next()) {
            if (solution->length < best_solution->length) {
                delete best_solution;
                best_solution = solution->clone();
            }
        }

        if (best_solution->length < current_length) {
            solution->set(best_solution);
            return true;
        } else {
            return false;
        }
    }

    while (ss.next()) {
        if (solution->length < current_length) {
            return true;
        } else if (moves > 1) {
            if (complexMove(solution, stop_time, best_improvement, moves-1, current_length)) {
                return true;
            }
        }
    }
    return false;
}

bool complexMove(Solution* solution, time_t stop_time, bool best_improvement) {
    int moves = 1;

    while (!time_up(stop_time)) {
        if (complexMove(solution, stop_time, best_improvement, moves, solution->length)) {
            return true;
        } else {
            moves++;
        }
    }

    return false;
}
