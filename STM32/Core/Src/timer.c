/*
 * timer.c
 *
 *  Created on: Nov 2, 2026
 *      Author: pham0
 */


#include "timer.h"
#include "input_reading.h"
#include "main.h"

int timer1_counter = 0;
int timer1_flag = 0;

int timer2_counter = 0;
int timer2_flag = 0;

int timer3_counter = 0;
int timer3_flag = 0;

void setTimer1(int duration_ms) {
    timer1_counter = duration_ms / TIMER_CYCLE;
    timer1_flag = 0;
}

void setTimer2(int duration_ms) {
    timer2_counter = duration_ms / TIMER_CYCLE;
    timer2_flag = 0;
}

void setTimer3(int duration_ms) {
    timer3_counter = duration_ms / TIMER_CYCLE;
    timer3_flag = 0;
}

void timerRun(void) {
    if (timer1_counter > 0) {
        timer1_counter--;
        if (timer1_counter <= 0) timer1_flag = 1;
    }
    if (timer2_counter > 0) {
        timer2_counter--;
        if (timer2_counter <= 0) timer2_flag = 1;
    }
    if (timer3_counter > 0) {
        timer3_counter--;
        if (timer3_counter <= 0) timer3_flag = 1;
    }
}

// Timer interrupt callback (called every 10ms by TIM2)
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM2) {
        button_reading();
        timerRun();
    }
}
