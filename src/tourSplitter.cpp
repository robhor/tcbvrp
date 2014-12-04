// Copyright 2014 Robert Horvath, Johannes Vogel
#include "./tourSplitter.h"
#include <vector>

TourSplitter::TourSplitter(Solution* solution) {
    this->solution = solution;
    this->split_begin = 2;
    this->split_end = 2;

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

bool TourSplitter::increment() {
    if (!buffer_tour) return false;

    split_end += 2;

    int split_begin_tour = solution->node_tour_index(split_begin);
    int split_end_tour   = solution->node_tour_index(split_end);
    int tour_length      = solution->tour_at(split_begin)->size();

    if (split_end_tour <= split_begin_tour || split_end_tour > tour_length - 2) {
        // split_end overflowed to next tour
        split_begin += 2;
        split_begin_tour += 2;
        while (solution->node_tour_index(split_begin) == 0) {
            split_begin += 2;
        }
        
        split_end = split_begin + 2;
    }

    int num_nodes = solution->instance->demand_nodes.size() * 2;
    return split_begin < num_nodes - 2;
}

void TourSplitter::split() {
    Tour* tour     = solution->tour_at(split_begin);
    int begin_index = solution->node_tour_index(split_begin);
    int end_index   = solution->node_tour_index(split_end);

    if (end_index == 0)
        end_index = solution->tour_at(split_begin)->size();

    solution->length -= solution->get_tour_cost(tour);
    
    buffer_tour->insert(buffer_tour->begin(),
        tour->begin()+begin_index, tour->begin()+end_index);
    tour->erase(tour->begin()+begin_index, tour->begin()+end_index);
    
    solution->length += solution->get_tour_cost(tour);
    solution->length += solution->get_tour_cost(buffer_tour);
}

Solution* TourSplitter::reset() {
    if (!buffer_tour) return nullptr;

    Tour* tour = solution->tour_at(split_begin-1);

    solution->length -= solution->get_tour_cost(tour);
    solution->length -= solution->get_tour_cost(buffer_tour);

    int begin_index = solution->node_tour_index(split_begin);
    if (begin_index == 0)
        begin_index = solution->tour_at(split_begin-1)->size();

    tour->insert(tour->begin()+begin_index,
        buffer_tour->begin(), buffer_tour->end());
    buffer_tour->erase(buffer_tour->begin(), buffer_tour->end());

    solution->length += solution->get_tour_cost(tour);

    return solution;
}

bool TourSplitter::next() {
    reset();
    if (!increment()) return false;
    split();
    return true;
}
