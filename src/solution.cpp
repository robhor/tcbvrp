// Copyright 2014 Robert Horvath, Johannes Vogel
#include "./solution.h"
#include <stdio.h>
#include <vector>

Solution::Solution() {
    length = 0;
}

int Solution::get_tour_cost(Tour *tour) {
    int cost = 0;
    int tour_size = tour->size();
    if (tour_size == 0) return cost;

    cost += instance->get_distance(0, tour->at(0));
    for (int i = 0; i < tour_size - 1; i++) {
        int from = tour->at(i);
        int to   = tour->at(i+1);
        int distance = instance->get_distance(from, to);
        cost += distance;
    }
    cost += instance->get_distance(tour->at(tour_size-1), 0);
    return cost;
}

int Solution::get_cost() {
    int cost = 0;
    for (auto tour : *tours) {
        cost += get_tour_cost(tour);
    }
    return cost;
}

void Solution::print(FILE* fd) {
    for (auto tour : *tours) {
        fprintf(fd, "0 ");
        for (auto node : *tour) fprintf(fd, "%i ", node);
        fprintf(fd, "0");
        fflush(fd);
        fprintf(stderr, " (%i)", get_tour_cost(tour));
        fprintf(fd, "\n");
    }
    fprintf(fd, "%i\n", length);

    if (length != get_cost()) {
        fprintf(stderr, "ERROR: This length is incorrect :O\n");
    }
}

void Solution::print() {
    print(stdout);
}

Solution* Solution::clone() {
    Solution* solution = new Solution;
    solution->instance = instance;
    solution->tours = new vector<Tour*>;
    solution->length = length;

    for (auto tour : *tours) {
        solution->tours->push_back(new Tour(tour->begin(), tour->end()));
    }

    return solution;
}

int Solution::node_at(int n) {
    for (auto tour : *tours) {
        int tour_size = tour->size();
        if (n < tour_size) {
            return tour->at(n);
        } else {
            n -= tour_size;
        }
    }
    return -1;
}

Tour* Solution::tour_at(int n) {
    for (auto tour : *tours) {
        int tour_size = tour->size();
        if (n < tour_size) {
            return tour;
        } else {
            n -= tour_size;
        }
    }
    return NULL;
}

int Solution::node_tour_index(int n) {
    for (auto tour : *tours) {
        int tour_size = tour->size();
        if (n < tour_size) {
            return n;
        } else {
            n -= tour_size;
        }
    }
    return -1;
}

void Solution::set_node_at(int n, int new_node) {
    for (auto tour : *tours) {
        int tour_size = tour->size();
        if (n < tour_size) {
            tour->at(n) = new_node;
            return;
        } else {
            n -= tour_size;
        }
    }
}

int Solution::predecessor_at(int n) {
    for (auto tour : *tours) {
        int tour_size = tour->size();
        if (n < tour_size) {
            if (n == 0) { return 0; }
            return tour->at(n-1);
        } else {
            n -= tour_size;
        }
    }
    return -1;
}

int Solution::successor_at(int n) {
    for (auto tour : *tours) {
        int tour_size = tour->size();
        if (n < tour_size) {
            if (n == tour_size - 1) { return 0; }
            return tour->at(n+1);
        } else {
            n -= tour_size;
        }
    }
    return -1;
}

void Solution::replace_node_at(int node_index, int new_node) {
    int node        = node_at(node_index);
    int predecessor = predecessor_at(node_index);
    int successor   = successor_at(node_index);

    length -= instance->get_distance(predecessor, node);
    length -= instance->get_distance(node, successor);

    length += instance->get_distance(predecessor, new_node);
    length += instance->get_distance(new_node, successor);

    set_node_at(node_index, new_node);
}

void Solution::insert_node(Tour* tour, int index, int node) {
    int predecessor, successor;
    int tour_size = tour->size();

    if (index == 0) {
        predecessor = 0;
    } else {
        predecessor = tour->at(index-1);
    }

    if (index > tour_size - 1) {
        successor = 0;
    } else {
        successor = tour->at(index);
    }

    length -= instance->get_distance(predecessor, successor);
    length += instance->get_distance(predecessor, node);
    length += instance->get_distance(node, successor);

    tour->insert(tour->begin()+index, node);
}

void Solution::remove_node(Tour* tour, int index) {
    int predecessor, successor;
    int tour_size = tour->size();

    if (index == 0) {
        predecessor = 0;
    } else {
        predecessor = tour->at(index-1);
    }

    if (index == tour_size - 1) {
        successor = 0;
    } else {
        successor = tour->at(index+1);
    }

    int node = tour->at(index);

    length -= instance->get_distance(predecessor, node);
    length -= instance->get_distance(node, successor);
    length += instance->get_distance(predecessor, successor);

    tour->erase(tour->begin()+index);
}
