// Copyright 2014 Robert Horvath, Johannes Vogel
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "./instance.h"
#include "./solution.h"
#include "./greedy.h"
#include "./randomGreedy.h"
#include "./nodeSwapHeuristic.h"
#include "./edgeMoveHeuristic.h"

void vnd(Solution* solution) {
    // perform Variable Neighborhood Descent
    int current_length = solution->length;
    fprintf(stderr, "Current solution length: %i", current_length);
    while (nodeSwap(solution) || edgeMove(solution)) {
        for (; current_length != 0; current_length /= 10) {
            fprintf(stderr, "\b");
        }
        current_length = solution->length;
        fprintf(stderr, "%i", current_length);
    }
    fprintf(stderr, "\n");
}

Solution* grasp(Instance* instance) {
    // Run randomized greedy heuristic
    Solution* solution = randomGreedy(instance, 0.4);
    fprintf(stderr, "Greedy Solution:\n");
    solution->print(stderr);

    return solution;
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

    Solution* solution = grasp(instance);
    vnd(solution);
    solution->print();
    return 0;
}
