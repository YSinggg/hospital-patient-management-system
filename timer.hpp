#ifndef TIMER_HPP
#define TIMER_HPP

#include <chrono>
using namespace std;

const int RUNS = 100;

double getTimeNow() {
    auto now = chrono::high_resolution_clock::now();
    return chrono::duration<double, micro>(now.time_since_epoch()).count();
}

#endif
