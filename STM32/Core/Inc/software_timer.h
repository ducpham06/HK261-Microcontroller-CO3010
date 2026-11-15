/*
 * software_timer.h
 *
 * Software timers driven by the TIM2 interrupt.
 * All durations are given in milliseconds, so changing the hardware tick
 * only requires updating TIMER_CYCLE (Lab 3, Exercise 4).
 */

#ifndef INC_SOFTWARE_TIMER_H_
#define INC_SOFTWARE_TIMER_H_

// TIM2 interrupt period in ms: 8 MHz / (7999 + 1) / (9 + 1) = 100 Hz -> 10 ms.
// Keep this value equal to the real TIM2 period.
#define TIMER_CYCLE 10

typedef enum {
    TIMER_COUNTDOWN = 0,    // 1 s tick of the normal-mode countdown
    TIMER_MANUAL_TIMEOUT,   // 10 s inactivity timeout in manual mode
    TIMER_BLINK,            // 250 ms toggle -> 2 Hz blinking in modification modes
    TIMER_SCAN,             // 7-segment multiplexing, one digit per expiry
    NUM_OF_TIMERS
} timer_id_t;

// Start (or restart) a timer. A duration shorter than one tick lasts one tick.
void setTimer(timer_id_t id, int duration_ms);

// Stop a timer and discard a pending expiry.
void clearTimer(timer_id_t id);

// Returns 1 once when the timer has expired, then clears the expiry.
int isTimerExpired(timer_id_t id);

// Called from the TIM2 interrupt every TIMER_CYCLE ms.
void timerRun(void);

#endif /* INC_SOFTWARE_TIMER_H_ */
