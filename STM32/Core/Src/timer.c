/*
 * timer.c
 *
 * TIM2 interrupt callback: runs every TIMER_CYCLE (10 ms).
 * Only fast, non-blocking work happens here; the FSMs run in the main loop.
 */

#include "main.h"
#include "software_timer.h"
#include "button.h"

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM2) {
        button_reading();
        timerRun();
    }
}
