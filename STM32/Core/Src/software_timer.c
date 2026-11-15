/*
 * software_timer.c
 *
 * Software timer implementation
 */

#include "software_timer.h"

static int timer_counter[NUM_OF_TIMERS] = {0};
static int timer_flag[NUM_OF_TIMERS] = {0};

void setTimer(timer_id_t id, int duration_ms) {
    if (id >= NUM_OF_TIMERS) return;
    // Round up to whole ticks so a timer never fires early
    int ticks = (duration_ms + TIMER_CYCLE - 1) / TIMER_CYCLE;
    if (ticks <= 0) ticks = 1;
    timer_flag[id] = 0;
    timer_counter[id] = ticks;
}

void clearTimer(timer_id_t id) {
    if (id >= NUM_OF_TIMERS) return;
    timer_counter[id] = 0;
    timer_flag[id] = 0;
}

int isTimerExpired(timer_id_t id) {
    if (id >= NUM_OF_TIMERS || timer_flag[id] == 0) return 0;
    timer_flag[id] = 0;
    return 1;
}

void timer_Run(void) {
    for (int i = 0; i < NUM_OF_TIMERS; i++) {
        if (timer_counter[i] > 0) {
            timer_counter[i]--;
            if (timer_counter[i] == 0) {
                timer_flag[i] = 1;
            }
        }
    }
}
