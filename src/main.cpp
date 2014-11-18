// Copyright 2014 Robert Horvath, Johannes Vogel
#include <stdio.h>
#include <string>
#include "./instance.h"

int main(int argc, char** argv) {
    if (argc == 1) {
        printf("USAGE: %s file\n", argv[0]);
        return 0;
    }

    Instance* instance = ReadInstanceFile(argv[1]);
    if (!instance) {
        fprintf(stderr, "The instance file could not be read: %s\n", argv[1]);
        return 1;
    }

    instance->print_summary();
    return 0;
}
