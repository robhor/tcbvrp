// Copyright 2014 Robert Horvath, Johannes Vogel
#include "./tourSplitter.h"
#include <vector>

TourSplitter::TourSplitter(Solution* solution) {
    this->solution = solution;
    this->split_index = 2;

    int num_vehicles = solution->instance->num_vehicles;
    int num_tours = solution->tours->size();

    if (num_tours < num_vehicles) {
        buffer_tour = new Tour;
        solution->tours->push_back(buffer_tour);
    } else {
        buffer_tour = nullptr;
    }
}

TourSplitter::~TourSplitter() {
    if (buffer_tour && buffer_tour->size() == 0) {
        solution->tours->pop_back();
        delete buffer_tour;
    }
}

Solution* TourSplitter::reset() {
    if (!buffer_tour) return nullptr;

    Tour* tour = solution->tour_at(split_index-1);

    solution->length -= solution->get_tour_cost(tour);
    solution->length -= solution->get_tour_cost(buffer_tour);

    tour->insert(tour->end(), buffer_tour->begin(), buffer_tour->end());
    buffer_tour->erase(buffer_tour->begin(), buffer_tour->end());

    solution->length += solution->get_tour_cost(tour);

    return solution;
}

bool TourSplitter::increment() {
    if (!buffer_tour) return false;

    split_index += 2;
    while (solution->node_tour_index(split_index) == 0) {
        split_index += 2;
    }

    int num_nodes = solution->instance->demand_nodes.size() * 2;
    return split_index < num_nodes - 2;
}

void TourSplitter::split() {
    Tour* tour     = solution->tour_at(split_index);
    int tour_index = solution->node_tour_index(split_index);

    solution->length -= solution->get_tour_cost(tour);

    buffer_tour->insert(buffer_tour->begin(),
        tour->begin()+tour_index, tour->end());
    tour->erase(tour->begin()+tour_index, tour->end());

    solution->length += solution->get_tour_cost(tour);
    solution->length += solution->get_tour_cost(buffer_tour);
}

bool TourSplitter::next() {
    reset();
    if (!increment()) return false;
    split();
    return true;
}
