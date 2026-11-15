/*
 * software_timer.h
 *
 * Software timers (from Lab 3). timer_Run() is a scheduler task that runs
 * every TIMER_CYCLE ms. Durations are given in milliseconds.
 */

#ifndef INC_SOFTWARE_TIMER_H_
#define INC_SOFTWARE_TIMER_H_

// Period of the timer_Run and getKeyInput tasks in ms
#define TIMER_CYCLE 10

typedef enum {
    TIMER_COUNTDOWN = 0,    // 1 s tick of the normal-mode countdown
    TIMER_MANUAL_TIMEOUT,   // 10 s inactivity timeout in manual mode
    TIMER_BLINK,            // 250 ms toggle -> 2 Hz blinking in modification modes
    NUM_OF_TIMERS
} timer_id_t;

// Start (or restart) a timer. A duration shorter than one tick lasts one tick.
void setTimer(timer_id_t id, int duration_ms);

// Stop a timer and discard a pending expiry.
void clearTimer(timer_id_t id);

// Returns 1 once when the timer has expired, then clears the expiry.
int isTimerExpired(timer_id_t id);

// Scheduler task, every TIMER_CYCLE ms
void timer_Run(void);

#endif /* INC_SOFTWARE_TIMER_H_ */
