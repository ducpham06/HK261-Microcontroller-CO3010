/*
 * button.h
 *
 * Debounced button input.
 *
 * getKeyInput() is a scheduler task that runs every TIMER_CYCLE ms. Each run
 * drops the presses nobody used in the previous tick, then samples the pins.
 * isButtonPressed() returns a press once and clears it, so when one FSM uses
 * a press (for example to change mode) the next FSM task cannot use it again.
 */

#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

#include "main.h"

// Button roles (Lab 3 handout, section 5.1)
typedef enum {
    BUTTON_MODE = 0,    // Button 1: select mode 1 -> 2 -> 3 -> 4 -> 1
    BUTTON_INC  = 1,    // Button 2: increase the value
    BUTTON_SET  = 2,    // Button 3: set (save) the value
    NO_OF_BUTTONS
} button_id_t;

#define BUTTON_IS_PRESSED   GPIO_PIN_RESET
#define BUTTON_IS_RELEASED  GPIO_PIN_SET

void button_init(void);

// Scheduler task, every TIMER_CYCLE ms
void getKeyInput(void);

// 1 once per press (or auto-repeat), then cleared
int isButtonPressed(button_id_t id);

#endif /* INC_BUTTON_H_ */
