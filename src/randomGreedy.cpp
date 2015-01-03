// Copyright 2014 Robert Horvath, Johannes Vogel
#include "./randomGreedy.h"
#include <stdio.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <set>
#include <limits>
#include <algorithm>
#include <iterator>


using std::set;

static unsigned int seedp = 0;

typedef struct partial_gtour {
    Instance *instance;
    Tour *tour;
    set<int> *used_nodes;
    int length;
} partial_gtour;

typedef struct distant_node {
    int node;
    int distance;
} distant_node;

/// Returns the nearest unused node out of the candidates to the given node
int nearest_node(partial_gtour *pt, int node, vector<int> candidates) {
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

int lastNode(partial_gtour *pt) {
    if (pt->length == 0) {
        return 0;
    } else {
        return pt->tour->back();
    }
}

bool supply_node_feasible(partial_gtour *pt, int node) {
    vector<int> dem = pt->instance->demand_nodes;
    set<int> *s2 = pt->used_nodes;
    set<int> unused_demands;

    std::set_difference(dem.begin(), dem.end(), s2->begin(), s2->end(),
        std::inserter(unused_demands, unused_demands.end()));

    int supply_distance = pt->instance->get_distance(lastNode(pt), node);
    for (int d : unused_demands) {
        int demand_distance = pt->instance->get_distance(node, d);
        int return_distance = pt->instance->get_distance(d, 0);
        int length = pt->length;
        length += supply_distance + demand_distance + return_distance;
        if (length <= pt->instance->time_limit) {
            return true;
        }
    }

    return false;
}

bool demand_node_feasible(partial_gtour *pt, int node) {
    int demand_distance = pt->instance->get_distance(lastNode(pt), node);
    int return_distance = pt->instance->get_distance(node, 0);
    int length = pt->length + demand_distance + return_distance;
    return length <= pt->instance->time_limit;
}

bool add_supply_node(partial_gtour *pt, float alpha) {
    vector<int> sup = pt->instance->supply_nodes;
    set<int> *s2 = pt->used_nodes;
    set<int> unused_supplies;

    std::set_difference(sup.begin(), sup.end(), s2->begin(), s2->end(),
        std::inserter(unused_supplies, unused_supplies.end()));

    // construct candidate list
    vector<distant_node*> candidate_list;
    int c_min, c_max;
    for (int i : unused_supplies) {
        if (supply_node_feasible(pt, i)) {
            distant_node *node = new distant_node;
            node->node = i;
            node->distance = pt->instance->get_distance(lastNode(pt), i);

            candidate_list.push_back(node);
            if (candidate_list.size() == 1) {
                c_min = node->distance;
                c_max = node->distance;
            }
            if (node->distance < c_min) {
                c_min = node->distance;
            }
            if (node->distance > c_max) {
                c_max = node->distance;
            }
        }
    }

    if (candidate_list.size() == 0)
        return false;

    // build restricted candidate list
    float limit = c_min + alpha * (c_max - c_min);
    vector<distant_node*> restricted_candidate_list;
    for (distant_node *node : candidate_list) {
        if (node->distance <= limit) {
            restricted_candidate_list.push_back(node);
        }
    }

    // choose random supply node of RCL
    vector<distant_node*>::const_iterator it(restricted_candidate_list.begin());
    int r = rand_r(&seedp) % restricted_candidate_list.size();
    std::advance(it, r);

    distant_node *node = *it;

    pt->length += node->distance;
    pt->tour->push_back(node->node);
    pt->used_nodes->insert(node->node);

    return true;
}

bool add_demand_node(partial_gtour *pt, float alpha) {
    vector<int> dem = pt->instance->demand_nodes;
    set<int> *s2 = pt->used_nodes;
    set<int> unused_demands;

    std::set_difference(dem.begin(), dem.end(), s2->begin(), s2->end(),
        std::inserter(unused_demands, unused_demands.end()));

    // construct candidate list
    vector<distant_node*> candidate_list;
    int c_min, c_max;
    for (int i : unused_demands) {
        if (demand_node_feasible(pt, i)) {
            distant_node *node = new distant_node;
            node->node = i;
            node->distance = pt->instance->get_distance(lastNode(pt), i);

            candidate_list.push_back(node);
            if (candidate_list.size() == 1) {
                c_min = node->distance;
                c_max = node->distance;
            }
            if (node->distance < c_min) {
                c_min = node->distance;
            }
            if (node->distance > c_max) {
                c_max = node->distance;
            }
        }
    }

    if (candidate_list.size() == 0)
        return false;

    // build restricted candidate list
    float limit = c_min + alpha * (c_max - c_min);
    vector<distant_node*> restricted_candidate_list;
    for (distant_node *node : candidate_list) {
        if (node->distance <= limit) {
            restricted_candidate_list.push_back(node);
        }
    }

    // choose random supply node of RCL
    vector<distant_node*>::const_iterator it(restricted_candidate_list.begin());
    int r = rand_r(&seedp) % restricted_candidate_list.size();
    std::advance(it, r);

    distant_node *node = *it;

    pt->length += node->distance;
    pt->tour->push_back(node->node);
    pt->used_nodes->insert(node->node);

    return true;
}

Tour* buildTour(Instance* instance, set<int> *used_nodes, float alpha) {
    Tour* tour = new Tour();

    partial_gtour pt;
    pt.instance = instance;
    pt.tour = tour;
    pt.used_nodes = used_nodes;
    pt.length = 0;

    while (add_supply_node(&pt, alpha)) {
        add_demand_node(&pt, alpha);
    }

    return tour;
}

Solution* randomGreedy(Instance* instance, float alpha) {
    Solution* solution = new Solution;
    solution->length = 0;
    solution->instance = instance;
    solution->tours = new vector<Tour*>;

    set<int> used_nodes;
    unsigned long num_used_nodes = 0;

    while (num_used_nodes < instance->demand_nodes.size() * 2) {
        Tour* tour = buildTour(instance, &used_nodes, alpha);
        num_used_nodes += tour->size();
        solution->tours->push_back(tour);
        solution->length += solution->get_tour_cost(tour);
    }

    return solution;
}
