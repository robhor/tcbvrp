// Copyright 2015 Robert Horvath, Johannes Vogel
#include "./antColony.h"
#include <math.h>
#include <stdlib.h>
#include <vector>
#include "./ant.h"
#include "./edgeMoveHeuristic.h"
#include "./nodeSwapHeuristic.h"

AntColony::AntColony(Instance* instance) {
    this->instance = instance;
    pheromone_state = new PheromoneState(instance);
    num_ants = 10;
}

Solution* best_solution(vector<Solution*> solutions) {
    Solution* best_solution = solutions.at(0);
    int best_length = best_solution->length;

    for (Solution* solution : solutions) {
        if (solution->length < best_length) {
            best_solution = solution;
            best_length = solution->length;
        }
    }

    return best_solution;
}

Solution* worst_solution(vector<Solution*> solutions) {
    Solution* worst_solution = solutions.at(0);
    int worst_length = worst_solution->length;

    for (Solution* solution : solutions) {
        if (solution->length > worst_length) {
            worst_solution = solution;
            worst_length = solution->length;
        }
    }

    return worst_solution;
}

Solution* AntColony::run() {
    // TODO(robhor): Proper stopping criteria
    Solution* best = new Solution(pheromone_state->get_instance());

    for (int i = 0; i < 2500; i++) {
        // Step 1: Let some ants run through the place
        run_ants();

        // Step 2: Optimize their routes
        daemon_actions();

        // Step 3: Update pheromone levels
        update_pheromones();

        Solution* it_best = best_solution(ant_solutions);
        if (best->length == 0 || best->length > it_best->length) {
            best->set(it_best);
        }
        fprintf(stderr, "#%i: %i, best: %i\n",
            i, it_best->length, best->length);
    }

    return best;
}

void AntColony::run_ants() {
    for (Solution* solution : ant_solutions) {
        delete solution;
    }
    ant_solutions.clear();

    for (int i = 0; i < num_ants; i++) {
        Solution* solution = Ant(pheromone_state).run();
        ant_solutions.push_back(solution);
    }
}

void AntColony::daemon_actions() {
    for (Solution* solution : ant_solutions) {
        while (nodeSwap(solution) || edgeMove(solution)) {
            solution->trim();
        }
    }
}

void AntColony::update_pheromones() {
    pheromone_state->evaporate(0.75);

    Solution* best  = best_solution(ant_solutions);
    Solution* worst = worst_solution(ant_solutions);
    int diff = best->length - worst->length;

    for (auto solution : ant_solutions) {
        double amount;
        // amount = 20 * pow((double)(worst->length - solution->length) / diff, 2);
        // amount *= 100 / solution->length;
        // double amount = 10000.0f / solution->length;
        PheromoneState* ps = pheromone_state;
        double amount = 10 * worst->length / solution->length;
        for (auto t : *solution->tours) {
            ps->increase_pheromone_level(0, t->at(0), amount);
            ps->increase_pheromone_level(t->back(), 0, amount);
            for (size_t i = 0; i < t->size()-1; i++) {
                ps->increase_pheromone_level(t->at(i), t->at(i+1), amount);
            }
        }
    }
}
