/*
 * timer.c
 *
 * TIM2 interrupt, every 10 ms: read the buttons and run the software timers.
 */

#include "main.h"
#include "button.h"
#include "software_timer.h"

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance != TIM2) return;
    getKeyInput();
    timer_Run();
}
