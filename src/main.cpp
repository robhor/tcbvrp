// Copyright 2014 Robert Horvath, Johannes Vogel
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string>

#include "./instance.h"
#include "./solution.h"
#include "./greedy.h"
#include "./nodeSwapHeuristic.h"
#include "./edgeMoveHeuristic.h"
#include "./supplySwapHeuristic.h"

static Solution* best_solution;
static time_t last_interrupt = 0;

void interrupt_handler(int signum) {
    fprintf(stderr, "Caught signal %d\n", signum);
    bool stop = (time(0) - last_interrupt) < 1;
    if (best_solution) {
        best_solution->print(stop ? stdout : stderr);
    }
    if (stop) exit(0);
    last_interrupt = time(0);
}


int main(int argc, char** argv) {
    if (argc == 1) {
        printf("USAGE: %s file\n", argv[0]);
        return 0;
    }

    Instance* instance = ReadInstanceFile(argv[1]);
    if (!instance) {
        fprintf(stderr, "The instance file could not be read: %s\n", argv[1]);
        return 1;
    }

    instance->print_summary();

    // Run the greedy heuristic
    Solution* solution = greedy(instance);
    fprintf(stderr, "Greedy Solution:\n");
    solution->print(stderr);
    best_solution = solution->clone();

    signal(SIGINT, interrupt_handler);

    // perform Variable Neighborhood Descent
    int current_length = solution->length;
    fprintf(stderr, "Current solution length: %i", current_length);
    while (nodeSwap(solution) || edgeMove(solution) || supplySwap(solution)) {
        for (; current_length != 0; current_length /= 10) {
            fprintf(stderr, "\b");
        }
        current_length = solution->length;
        fprintf(stderr, "%i", current_length);
        Solution* new_best = solution->clone();
        delete best_solution;
        best_solution = new_best;
    }
    fprintf(stderr, "\n");

    // fprintf(stderr, "\nNodeSwapper Solution:\n");
    best_solution->print();

    return 0;
}
