/*
 * software_timer.h
 */

#ifndef INC_SOFTWARE_TIMER_H_
#define INC_SOFTWARE_TIMER_H_

#define TIMER_CYCLE         10      // timer_Run is called every 10 ms

#define TIMER_COUNTDOWN     0       // 1 s countdown in mode 1
#define TIMER_MANUAL        1       // 10 s idle timeout in manual mode
#define TIMER_BLINK         2       // 2 Hz blinking in modes 2-4
#define TIMER_SCAN          3       // 7-segment scanning (Lab 3 only)
#define NUM_OF_TIMERS       4

void setTimer(int index, int duration_ms);
int isTimerExpired(int index);      // 1 once when expired, then cleared
void timer_Run(void);

#endif /* INC_SOFTWARE_TIMER_H_ */
