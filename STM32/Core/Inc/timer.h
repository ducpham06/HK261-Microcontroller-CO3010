/*
 * timer.h
 *
 *  Created on: Nov 2, 2026
 *      Author: pham0
 */

#ifndef INC_TIMER_H_
#define INC_TIMER_H_

#define TIMER_CYCLE 10 // Hardware timer tick in ms

// Software timer flags and counters
extern int timer1_flag; // 1s timer for normal mode countdown
extern int timer2_flag; // 250ms toggle timer for 2 Hz LED blinking
extern int timer3_flag; // 5ms timer for 7-segment multiplexing

void setTimer1(int duration_ms);
void setTimer2(int duration_ms);
void setTimer3(int duration_ms);
void timerRun(void);

#endif /* INC_TIMER_H_ */
