#ifndef LOGGING_H
#define LOGGING_H
#include <stdio.h>

//define an info function based on verbosity level
#ifdef SILENT
#define info(fmt_, ...) {}
#else
#define info(fmt_, ...) {printf(fmt_, ##__VA_ARGS__);}
#endif

//define an error function that prints a message to stderr and then aborts
#define error(fmt_, ...) {fprintf(stderr,"ERROR: "); fprintf(stderr, fmt_, ##__VA_ARGS__); exit(-1);}

#endif
