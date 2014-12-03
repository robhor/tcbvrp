// Copyright 2014 Robert Horvath, Johannes Vogel
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "./instance.h"
#include "./solution.h"
#include "./greedy.h"
#include "./nodeSwapper.h"

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
    solution->print();

    // swap nodes heuristic
    int best_length = solution->length;
    NodeSwapper ns(solution);
    while (true) {
        while ((solution = ns.next()) != NULL) {
            if (solution->length < best_length) {
                best_length = solution->length;
                break;
            }
        }
        if (solution != NULL) {
            ns.accept();
        } else {
            solution = ns.reset();
            break;
        }
    }

    fprintf(stderr, "\nNodeSwapper Solution:\n");
    solution->print();

    return 0;
}
