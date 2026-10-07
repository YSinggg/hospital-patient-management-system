// ============================================================
//  timer.hpp
//  Used by BOTH programs.
//  A simple way to measure how long something takes.
//
//  How to use:
//      double start = getTimeNow();
//      ... the code you want to measure ...
//      double timeTaken = getTimeNow() - start;   // in microseconds
// ============================================================
#ifndef TIMER_HPP
#define TIMER_HPP

#include <chrono>
using namespace std;

const int RUNS = 100;   // how many times each test is repeated in the performance test

// Returns the current time in microseconds (1 second = 1,000,000 microseconds).
// steady_clock is used because it never jumps forwards or backwards.
double getTimeNow() {
    return chrono::duration<double, micro>(chrono::steady_clock::now().time_since_epoch()).count();
}

#endif
