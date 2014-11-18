#include "instance.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

void Instance::init() {
	int nodes = num_nodes + 1; // including special node 0
	distances = (int*) malloc(sizeof(int) * nodes * nodes);
}

void Instance::set_distance(int from, int to, int distance) {
	distances[from * num_nodes + to] = distance;
}

int Instance::get_distance(int from, int to) {
	return distances[from * num_nodes + to];
}

void Instance::print_summary() {
	cout << "Nodes:      " << num_nodes << endl;
	cout << "Time Limit: " << time_limit << endl;
	cout << "Vehicles:   " << num_vehicles << endl;
}

Instance::~Instance() {
	if (distances != NULL) free(distances);
}



Instance* ReadInstanceFile(char* path) {
	ifstream infile(path);
	
	Instance *instance = new Instance;
	
	int num_nodes;
	infile >> num_nodes;
	instance->num_nodes = num_nodes;
	infile >> instance->time_limit;
	infile >> instance->num_vehicles;
	instance->init();

	// Read nodes
	int node, i = 0;
	char type;
	while (i < num_nodes) {
		infile >> node >> type;
		if (type == 'S') {
			instance->supply_nodes.push_back(node);
		} else {
			instance->demand_nodes.push_back(node);
		}
		i++;
	}

	// Read distance matrix
	int distance;
	i = 0;
	while (infile >> distance) {
		int from = i / (num_nodes + 1);
		int to = i % (num_nodes + 1);
		instance->set_distance(from, to, distance);
		i++;
	}

	return instance;
}