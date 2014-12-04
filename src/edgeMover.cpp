// Copyright 2014 Robert Horvath, Johannes Vogel
#include "./edgeMover.h"

void extend_solution_tours(Solution* solution) {
    int num_tours = solution->tours->size();
    if (solution->tours->at(num_tours-1)->empty()) return;
    if (num_tours < solution->instance->num_vehicles) {
        // enough vehicles for another tour!
        solution->tours->push_back(new Tour);
    }
}

EdgeMover::EdgeMover(Solution* solution) {
    this->solution = solution;
    extend_solution_tours(solution);

    src_tour = 0;
    src = 0;

    dst_tour = 0;
    dst = 0;
}

EdgeMover::~EdgeMover() {
    if (src_tour > dst_tour) {
        int tmp_tour = src_tour;
        src_tour = dst_tour;
        dst_tour = tmp_tour;
    }

    Tour* t;

    int num_tours = solution->tours->size();
    t = solution->tours->at(num_tours-1);
    if (t->size() == 0) {
        solution->tours->pop_back();
        delete t;
        num_tours--;
    }

    if (dst_tour < num_tours) {
        t = solution->tours->at(dst_tour);
        if (t->size() == 0) {
            solution->tours->erase(solution->tours->begin()+dst_tour);
            delete t;
        }
    }

    num_tours = solution->tours->size();
    if (src_tour < num_tours) {
        t = solution->tours->at(src_tour);
        if (t->size() == 0) {
            solution->tours->erase(solution->tours->begin()+src_tour);
            delete t;
        }
    }
}

void EdgeMover::accept() {
    src_tour = 0;
    src = 0;

    dst_tour = 0;
    dst = 0;

    extend_solution_tours(solution);
}

Solution* EdgeMover::reset() {
    if (src_tour == 0 && src == 0 && dst_tour == 0 && dst == 0)
        return solution;

    if (src_tour == dst_tour && src == dst)
        return solution;


    int r_src = src;
    int r_dst = dst;

    if (dst_tour == src_tour) {
        if (dst < src) r_src += 2;
        else
            r_dst -= 2;
    }

    move_edge(dst_tour, r_dst, src_tour, r_src);
    return solution;
}

/// Increment indices. Returns false if no more indices to consider
bool EdgeMover::increment() {
    int num_tours = solution->tours->size();

    if (dst_tour == num_tours) return false;

    int src_tour_size = solution->tours->at(src_tour)->size();
    int dst_tour_size = solution->tours->at(dst_tour)->size();
    dst += 2;

    if (dst > dst_tour_size) {
        dst_tour++;
        dst = src % 2;

        while (dst_tour < num_tours) {
            dst_tour_size = solution->tours->at(dst_tour)->size();
            if (dst_tour_size >= dst) break;
            else
                dst_tour++;
        }
    }

    if (dst_tour >= num_tours) {
        src++;
        if (src > src_tour_size - 2) {
            src = 0;
            src_tour++;
            while (src_tour < num_tours) {
                src_tour_size = solution->tours->at(src_tour)->size();
                if (src_tour_size >= 2) break;
                else
                    src_tour++;
            }
            if (src_tour >= num_tours) return false;
        }

        dst_tour = 0;
        dst = src % 2;
    }

    return true;
}

void EdgeMover::move_edge(int src_tour, int src, int dst_tour, int dst) {
    if (src_tour == dst_tour && src == dst) return;

    // First, insert at new location
    // Then, delete at current location
    int node_a = solution->tours->at(src_tour)->at(src);
    int node_b = solution->tours->at(src_tour)->at(src + 1);

    Tour* src_tour_ptr = solution->tours->at(src_tour);
    Tour* dst_tour_ptr = solution->tours->at(dst_tour);

    solution->insert_node(dst_tour_ptr, dst, node_b);
    solution->insert_node(dst_tour_ptr, dst, node_a);

    if (dst_tour == src_tour && dst < src)
        src += 2;

    solution->remove_node(src_tour_ptr, src);
    solution->remove_node(src_tour_ptr, src);
}

bool EdgeMover::move_valid() {
    Tour* src_tour_ptr = solution->tours->at(src_tour);
    Tour* dst_tour_ptr = solution->tours->at(dst_tour);

    int src_tour_cost = solution->get_tour_cost(src_tour_ptr);
    int dst_tour_cost = solution->get_tour_cost(dst_tour_ptr);

    return src_tour_cost <= solution->instance->time_limit &&
           dst_tour_cost <= solution->instance->time_limit;
}

Solution* EdgeMover::next() {
    while (true) {
        reset();
        if (!increment()) return nullptr;
        move_edge(src_tour, src, dst_tour, dst);
        if (move_valid()) return solution;
    }
}
