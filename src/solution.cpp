// Copyright 2014 Robert Horvath, Johannes Vogel
#include "./solution.h"

int Solution::get_tour_cost(Tour *tour) {
    int cost = 0;
    int tour_size = tour->size();
    for (int i = 0; i < tour_size - 1; i++) {
        cost += instance->get_distance(tour->at(i), tour->at(i+1));
    }
    return cost;
}

int Solution::get_cost() {
    int cost = 0;
    for (auto tour : *tours) {
        cost += get_tour_cost(tour);
    }
    return cost;
}

void Solution::print() {
    for (auto tour : *tours) {
        for (auto node : *tour) printf("%i ", node);
        printf("0\n");
    }
    printf("%i\n", get_cost());
}
