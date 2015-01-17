// Copyright 2015 Robert Horvath, Johannes Vogel
#include "./antColony.h"
#include <math.h>
#include <stdlib.h>
#include <vector>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include "./ant.h"
#include "./edgeMoveHeuristic.h"
#include "./nodeSwapHeuristic.h"


#define NUM_ANTS 20
#define NUM_GENERATIONS 100
#define NUM_DAEMON_ITERATIONS 100

int pheromone_model = 1;

AntColony::AntColony(Instance* instance) {
    this->instance = instance;
    pheromone_state = new PheromoneState(instance);
    num_ants = NUM_ANTS;
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
    Solution* best = new Solution(pheromone_state->get_instance());

    for (int i = 0; i < NUM_GENERATIONS; i++) {
        // Step 1: Let some ants run through the place
        run_ants();

        // Step 2: Optimize their routes
        //daemon_actions(); // included in run_ants

        // Step 3: Update pheromone levels
        update_pheromones(pheromone_model);

        Solution* it_best = best_solution(ant_solutions);
        if (best->length == 0 || best->length > it_best->length) {
            best->set(it_best);
        }
        fprintf(stderr, "#%i: %i, best: %i\n", i, it_best->length, best->length);
    }

    return best;
}

void optimize_solution(Solution* solution) {
    int rounds = NUM_DAEMON_ITERATIONS;
    while (rounds-- > 0 && (nodeSwap(solution) || edgeMove(solution))) {
        solution->trim();
    }
}

void AntColony::run_ant() {
    Solution* solution = Ant(pheromone_state).run();
    optimize_solution(solution);
    mtx.lock();
    ant_solutions.push_back(solution);
    mtx.unlock();
}

void AntColony::run_ants() {
    for (Solution* solution : ant_solutions) {
        delete solution;
    }
    ant_solutions.clear();
    boost::thread_group tgroup;

    for (int i = 0; i < num_ants; i++) {
        tgroup.create_thread(boost::bind(&AntColony::run_ant, this));
    }
    tgroup.join_all();
}
void AntColony::daemon_actions() {
    boost::thread_group tgroup;
    for (Solution* solution : ant_solutions) {
        tgroup.create_thread(boost::bind(optimize_solution, solution));
    }
    tgroup.join_all();
}
/*
void AntColony::update_pheromones() {
    pheromone_state->evaporate(0.75);

    Solution* best  = best_solution(ant_solutions);
    Solution* worst = worst_solution(ant_solutions);
    //int diff = best->length - worst->length;

    for (auto solution : ant_solutions) {
        // double amount;
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
}*/

void AntColony::update_pheromones(int modelNr){
    switch(modelNr){
        case 1: update_pheromones_1(); break;
        case 2: update_pheromones_2(); break;
        default:return;
    }
}

void AntColony::update_pheromones_1() {
    pheromone_state->evaporate(0.75);

    //Solution* best  = best_solution(ant_solutions);
    Solution* worst = worst_solution(ant_solutions);
    //int diff = best->length - worst->length;

    for (auto solution : ant_solutions) {
        // double amount;
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
void AntColony::update_pheromones_2(){
    pheromone_state->evaporate(0.75);

    Solution* best  = best_solution(ant_solutions);
    Solution* worst = worst_solution(ant_solutions);
    int diff = best->length - worst->length;

    for (auto solution : ant_solutions) {
        // double amount;
        double amount = 20 * pow((double)(worst->length - solution->length) / diff,2.5);
        amount += pow(diff,1/5);
        // amount *= 100 / solution->length;
        // double amount = 10000.0f / solution->length;
        PheromoneState* ps = pheromone_state;
        //double amount = 10 * worst->length / solution->length;
        for (auto t : *solution->tours) {
            ps->increase_pheromone_level(0, t->at(0), amount);
            ps->increase_pheromone_level(t->back(), 0, amount);
            for (size_t i = 0; i < t->size()-1; i++) {
                ps->increase_pheromone_level(t->at(i), t->at(i+1), amount);
            }
        }
    }
}
