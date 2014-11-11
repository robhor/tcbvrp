#include <iostream>
#include <string>
#include <fstream>
using namespace std;


int main(int argc, char** argv) {
	if (argc == 1) {
		cout << "USAGE: " << argv[0] << " file" << endl;
		return 0;
	}

	ifstream infile(argv[1]);
	int nodes, timeLimit, vehicles;
	infile >> nodes >> timeLimit >> vehicles;
	
	cout << "Nodes: \t" << nodes << endl;
	cout << "Time Limit: \t" << timeLimit << endl;
	cout << "Vehicles: \t" << vehicles << endl;

	return 0;
}
