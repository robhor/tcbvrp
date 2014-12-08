// Copyright 2014 Robert Horvath, Johannes Vogel
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string>
#include <ctime>

#include "./instance.h"
#include "./solution.h"
#include "./greedy.h"
#include "./randomGreedy.h"
#include "./complexMoveHeuristic.h"

static Solution* best_solution;
static time_t last_interrupt = 0;
static int timeout = 10;

void interrupt_handler(int signum) {
    fprintf(stderr, "Caught signal %d\n", signum);
    bool stop = (time(0) - last_interrupt) < 1;
    if (best_solution) {
        best_solution->print(stop ? stdout : stderr);
    }
    if (stop) exit(0);
    last_interrupt = time(0);
}

Solution* vnd(Solution* solution) {
    // Perform Variable Neighborhood Descent
    int current_length = solution->length;
    fprintf(stderr, "Current solution length: %i\n", current_length);
    time_t start_time = time(0);
    time_t time_to_stop = start_time + timeout;

    while (complexMove(solution, time_to_stop)) {
        solution->trim();
        current_length = solution->length;
        fprintf(stderr, "Current solution length: %i (%lus)\n",
            current_length, time(0)-start_time);
        time_to_stop = time(0) + timeout;
        
        Solution* new_best = solution->clone();
        delete best_solution;
        best_solution = new_best;
    }
    fprintf(stderr, "\n");
    return best_solution;
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
        printf("USAGE: %s file [timeout]\n", argv[0]);
        return 0;
    }

    Instance* instance = ReadInstanceFile(argv[1]);
    if (!instance) {
        fprintf(stderr, "The instance file could not be read: %s\n", argv[1]);
        return 1;
    }

    timeout = 3 * instance->num_nodes;
    if (argc >= 3) {
        timeout = atoi(argv[2]);
    }
    if (timeout < 1) { timeout = 1; }

    instance->print_summary();

    bool randomized_construction = true;
    Solution* solution;

    if (randomized_construction) {
        solution = grasp(instance);
    } else {
        solution = greedy(instance);
    }
    fprintf(stderr, "Greedy Solution:\n");
    solution->print(stderr);
    signal(SIGINT, interrupt_handler);

    solution = vnd(solution);
    solution->print();

    return 0;
}
