#include "software_timer.h"

static volatile int timer_counter[NUM_OF_TIMERS];
static volatile int timer_flag[NUM_OF_TIMERS];

void setTimer(int index, int duration_ms) {
    timer_flag[index] = 0;
    timer_counter[index] = (duration_ms + TIMER_CYCLE - 1) / TIMER_CYCLE;
}

int isTimerExpired(int index) {
    if (timer_flag[index] == 0) return 0;
    timer_flag[index] = 0;
    return 1;
}

void timer_Run(void) {
    for (int i = 0; i < NUM_OF_TIMERS; i++) {
        if (timer_counter[i] <= 0) continue;
        timer_counter[i]--;
        if (timer_counter[i] == 0) timer_flag[i] = 1;
    }
}
