#include <iostream>
#include <string>


#include "instance.h"

using namespace std;

int main(int argc, char** argv) {
	if (argc == 1) {
		cout << "USAGE: " << argv[0] << " file" << endl;
		return 0;
	}

	Instance* instance = ReadInstanceFile(argv[1]);
	if (!instance) {
		cerr << "The instance file could not be read: " << argv[1] << endl;
		return 1;
	}

	instance->print_summary();
	return 0;
}
