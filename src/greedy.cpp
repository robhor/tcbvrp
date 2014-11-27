// Copyright 2014 Robert Horvath, Johannes Vogel
#include "./greedy.h"
#include <stdio.h>
#include <vector>
#include <set>
#include <limits>

using std::set;

typedef struct partial_tour {
    Instance *instance;
    Tour *tour;
    set<int> *used_nodes;
    int length;
} partial_tour;

/// Returns the nearest unused node out of the candidates to the given node
int nearest_node(partial_tour *pt, int node, vector<int> candidates) {
    int min_distance = std::numeric_limits<int>::max();
    int nearest_node = 0;

    for (auto current_node : candidates) {
        if (pt->used_nodes->find(current_node) != pt->used_nodes->end()) {
            // node already used
            continue;
        }

        int distance = pt->instance->get_distance(node, current_node);
        if (distance < min_distance) {
            min_distance = distance;
            nearest_node = current_node;
        }
    }

    return nearest_node;
}

/**
 * Extend the tour by the nearest supply+demand.
 * Returns false if no supply+demand can be added anymore
 * while still being able to return to the depot in time.
 * Otherwise, return true.
 */
bool greedy_extend_tour(partial_tour *pt) {
    int last_node = 0;
    if (pt->tour->size() > 0) {
        last_node = pt->tour->back();
    }

    int supply_node =
        nearest_node(pt, last_node, pt->instance->supply_nodes);

    if (supply_node == 0) {
        // No supply nodes left
        pt->length += pt->instance->get_distance(last_node, 0);
        return false;
    }

    int supply_cost = pt->instance->get_distance(last_node, supply_node);

    int demand_node =
        nearest_node(pt, supply_node, pt->instance->demand_nodes);

    if (demand_node == 0) {
        // No demand nodes left
        pt->length += pt->instance->get_distance(last_node, 0);
        return false;
    }

    int demand_cost = pt->instance->get_distance(supply_node, demand_node);
    int return_cost = pt->instance->get_distance(demand_node, 0);

    int new_length = pt->length + supply_cost + demand_cost + return_cost;
    if (new_length > pt->instance->time_limit) {
        // Segment too long, cannot add it to tour
        pt->length += pt->instance->get_distance(last_node, 0);
        return false;
    }

    pt->tour->push_back(supply_node);
    pt->tour->push_back(demand_node);
    pt->used_nodes->insert(supply_node);
    pt->used_nodes->insert(demand_node);
    pt->length = pt->length + supply_cost + demand_cost;

    return true;
}

Solution* greedy(Instance* instance) {
    Solution* solution = new Solution;
    solution->length = 0;
    solution->instance = instance;
    solution->tours = new vector<Tour*>;

    // Be greedy. Be very greedy.
    set<int> used_nodes;
    used_nodes.insert(0);  // Never add the depot to a tour

    int num_used_nodes = used_nodes.size();
    while (num_used_nodes < instance->num_nodes + 1) {
        int prev_num_used_nodes = num_used_nodes;
        Tour* tour = new Tour();

        partial_tour pt;
        pt.instance = instance;
        pt.tour = tour;
        pt.used_nodes = &used_nodes;
        pt.length = 0;

        while (greedy_extend_tour(&pt)) {}
        if (pt.tour->size() > 1) {
            solution->tours->push_back(tour);
            solution->length += pt.length;
        }

        num_used_nodes = used_nodes.size();
        if (prev_num_used_nodes >= num_used_nodes) break;
    }

    return solution;
}
