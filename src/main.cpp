// Copyright 2014 Robert Horvath, Johannes Vogel
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <boost/program_options.hpp>
#include <string>
#include <ctime>
#include <iostream>

#include "./instance.h"
#include "./solution.h"
#include "./greedy.h"
#include "./randomGreedy.h"
#include "./complexMoveHeuristic.h"
#include "./antColony.h"

static Solution* best_solution;
static int best_solution_time = 0;
static time_t last_interrupt = 0;
static float alpha = 0.4;
static int timeout = 10;
static bool randomized = false;
static bool best_improvement = false;
static time_t global_start_time;

void interrupt_handler(int signum) {
    fprintf(stderr, "Caught signal %d\n", signum);
    bool stop = (time(0) - last_interrupt) < 1;
    if (best_solution) {
        best_solution->print(stop ? stdout : stderr);
    }
    if (stop) exit(0);
    last_interrupt = time(0);
}

void set_best_solution(Solution* solution) {
    if (best_solution && best_solution->length <= solution->length) {
        return;
    }
    best_solution_time = time(0);
    if (best_solution && best_solution != solution) {
        delete best_solution;
    }
    best_solution = solution->clone();
}

Solution* vnd(Solution* solution) {
    // Perform Variable Neighborhood Descent
    int current_length = solution->length;
    fprintf(stderr, "Current solution length: %i\n", current_length);
    time_t start_time = time(0);
    time_t time_to_stop = start_time + timeout;

    while (complexMove(solution, time_to_stop, best_improvement)) {
        solution->trim();
        current_length = solution->length;
        fprintf(stderr, "Current solution length: %i (%lus / %lus)\n",
            current_length, time(0)-start_time, time(0)-global_start_time);
        time_to_stop = time(0) + timeout;

        set_best_solution(solution);
    }
    return solution;
}

Solution* grasp(Instance* instance) {
    for (int i = 0; i < 30; i++) {
        Solution* solution = randomGreedy(instance, alpha);

        fprintf(stderr, "Greedy Solution Nr. %i:\n", i);
        solution->print(stderr);

        solution = vnd(solution);
        solution->print(stderr);
    }
    fprintf(stderr, "Best solution found in %lus, total runtime %lus:\n",
        best_solution_time-global_start_time, time(0)-global_start_time);
    best_solution->print();
    return best_solution;
}

/// Normal old solving, no GRASP
void solve(Instance* instance) {
    Solution* solution;

    if (randomized) {
        solution = randomGreedy(instance, alpha);
    } else {
        solution = greedy(instance);
    }

    fprintf(stderr, "Greedy Solution:\n");
    solution->print(stderr);
    set_best_solution(solution);
    signal(SIGINT, interrupt_handler);

    solution = vnd(solution);
    fprintf(stderr, "Best solution found in %lus, total runtime %lus:\n",
        best_solution_time-global_start_time, time(0)-global_start_time);
    solution->print();
}

int main(int argc, char** argv) {
    namespace po = boost::program_options;
    po::options_description description
        ("Time-Constrained Bepartite Vehicle Routing Problem Solver");

    description.add_options()
        ("timeout,t", po::value<int>()->implicit_value(10),
            "Timeout after new solution has been found")
        ("randomized,r", "Use a randomized construction heuristic")
        ("alpha,a", po::value<float>(), "Alpha value for random construction heuristic [0-1]")
        ("grasp,g", "Use GRASP")
        ("best-improvement,b", "Use best-improvement instead of first-improvement")
        ("help,h", "Display this help message")
        ("instance,i", po::value<std::string>(), "Instance file")
        ("version,v", "Display the version number")
        ("ant1,a1","Use ant colony heuristic with first pheromone model")
        ("ant2,a2","Use ant colony heuristic with first pheromone model")
        ("p=2","Use second pheromone model");


    po::positional_options_description p;
    p.add("instance", 1);
    po::variables_map vm;
    try {
        po::store(po::command_line_parser(argc, argv).options(description).positional(p).run(), vm);
        po::notify(vm);
    } catch(boost::program_options::error& e) {
        std::cout << description;
        exit(1);
    }

    if (vm.count("version")) {
        printf("1.0\n");
        exit(0);
    } else if (vm.count("help") || !vm.count("instance")) {
        std::cout << description;
        exit(0);
    }

    std::string file = vm["instance"].as<std::string>();
    Instance* instance = ReadInstanceFile(file.c_str());
    instance->print_summary();
    srand(time(0));

    // FIXME(robhor): Activate through some command line option
    global_start_time = time(0);
    if(vm.count("ant1")){
            AntColony* ants = new AntColony(instance);
            ants->pheromone_model = 1;
            time_t start_time = time(0);
            Solution* sol = ants->run();
            time_t end_time = time(0);
            double time_in_seconds = difftime(end_time, start_time);
            sol->print();
            fprintf(stderr, "Time in seconds: %f\n", time_in_seconds);
            return 0;
        
    }

    if(vm.count("ant1")){
            AntColony* ants = new AntColony(instance);
            ants->pheromone_model = 2;
            Solution* sol = ants->run();
            sol->print(); 
            return 0;       
    }

    if (vm.count("timeout")) {
        timeout = vm["timeout"].as<int>();
    } else if (vm.count("grasp")) {
        timeout = instance->num_nodes / 2;
    } else {
        timeout = 3 * instance->num_nodes;
    }
    fprintf(stderr, "Timeout: %is\n", timeout);

    if (vm.count("randomized")) {
        randomized = true;
    }

    if (vm.count("alpha")) {
        alpha = vm["alpha"].as<float>();
        if (alpha < 0) alpha = 0;
        else if (alpha > 1) alpha = 1;
    }

    if (vm.count("best-improvement")) {
        best_improvement = true;
    }

    //global_start_time = time(0);
    if (vm.count("grasp")) {
        grasp(instance);
    } else {
        solve(instance);
    }



    return 0;
}
