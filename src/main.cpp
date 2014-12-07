// Copyright 2014 Robert Horvath, Johannes Vogel
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <ctime>
#include "./instance.h"
#include "./solution.h"
#include "./greedy.h"
#include "./nodeSwapHeuristic.h"
#include "./edgeMoveHeuristic.h"
#include "./complexMoveHeuristic.h"

int main(int argc, char** argv) {
    if (argc == 1) {
        printf("USAGE: %s file [timeout]\n", argv[0]);
        return 0;
    }

    Instance* instance = ReadInstanceFile(argv[1]);
    if (!instance) {
        fprintf(stderr, "The instance file could not be read: %s\n", argv[1]);
        return 1;
    }

    int timeout = 10;
    if (argc >= 3) {
        timeout = atoi(argv[2]);
    }
    if (timeout < 1) { timeout = 1; }

    instance->print_summary();

    // Run the greedy heuristic
    Solution* solution = greedy(instance);
    fprintf(stderr, "Greedy Solution:\n");
    solution->print(stderr);


    // Perform Variable Neighborhood Descent
    int current_length = solution->length;
    fprintf(stderr, "Current solution length: %i", current_length);
    time_t time_to_stop = time(nullptr) + timeout;

    while (complexMove(solution, time_to_stop)) {
        solution->trim();
        for (; current_length != 0; current_length /= 10) {
            fprintf(stderr, "\b");
        }
        current_length = solution->length;
        time_to_stop = time(nullptr) + timeout;
        fprintf(stderr, "%i", current_length);
    }
    fprintf(stderr, "\n");

    solution->print();
    return 0;
}
