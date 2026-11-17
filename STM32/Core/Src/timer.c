/*
 * timer.c
 *
 * TIM2 interrupt, every 10 ms: only tell the scheduler a tick has passed.
 */

#include "main.h"
#include "scheduler.h"

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance != TIM2) return;
    SCH_Update();
}
