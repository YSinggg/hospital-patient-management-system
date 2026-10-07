#ifndef TIMER_HPP
#define TIMER_HPP

#include <chrono>
using namespace std;

const int RUNS = 100;   // how many times each test is repeated in the performance test

// Returns the current time in microseconds (1 second = 1,000,000 microseconds).
double getTimeNow() {
    return chrono::duration<double, micro>(chrono::steady_clock::now().time_since_epoch()).count();
}

#endif
