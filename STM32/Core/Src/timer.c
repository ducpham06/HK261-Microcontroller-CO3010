/*
 * timer.c
 *
 * TIM2 interrupt (timer_isr): every SCH_TICK_MS (10 ms) it only tells the
 * scheduler that one tick has passed. All other work runs as tasks.
 */

#include "main.h"
#include "scheduler.h"

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM2) {
        SCH_Update();
    }
}
