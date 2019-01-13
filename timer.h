#ifndef TIMER_H
#define TIMER_H
#include "logging.h"

//function to start a new timer
//note that timers can be nested!
void timer_start();

//get current time passed in us
long int timer_get();

//stop timer and get passed time in us
long int timer_stop();

//destroy the current timer object (if nested the previous one will become active)
void timer_destroy();

#ifdef TIMING

//timeit macro to be wrapped around what should be timed
#define timeit_named(name, func) \
    info("Starting timer %s\n----------\n", name);\
    timer_start();\
    func;\
    info("----------\nEnding timer %s\nTime taken: %ld us (%lf seconds)\n",name, timer_stop(), (double)timer_stop()/1000000.0 );\
    timer_destroy();

//time with empty name
#define timeit(func) timeit_named("", func);


#else

//Dummy defines - nothing will be timed
#define timeit_named(name, func) func
#define timeit(func) func

#endif


///////////////////////////////////////
//example usage of the 'timeit' macro

//#include <unistd.h> //include for sleep
//timeit(
//    info("Toplevel timer\n");
//    timeit(
//        info("inner timer\n");
//        sleep(1);
//    );
//    sleep(1);
//);

#endif
