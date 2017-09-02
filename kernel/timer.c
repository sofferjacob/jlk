#include <timer.h>
#include <stdint.h>
#include <hal.h>


void start_timer(float time, void(*listener)()) {
    size_t time_at_start = readSystemTime();
    float total_time = (time * TICKSTOSEC) + time_at_start;  // Every 100 ticks is a second in x86
    while (readSystemTime() < total_time) {
        // Wait for the times to match
    }
    // TODO: Use an interrupt to execute listener
    listener();
}

void delay(float time) {
    size_t time_at_start = readSystemTime();
    float total_time = (time * TICKSTOSEC) + time_at_start; // Every 100 ticks is a second in x86
    while (readSystemTime() < total_time) {
        // Wait for the times to match
    }
}