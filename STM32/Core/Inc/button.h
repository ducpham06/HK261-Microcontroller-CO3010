/*
 * button.h
 *
 * Debounced button input.
 *
 * button_reading() samples the pins in the TIM2 interrupt. The main loop
 * calls button_collect_events() once per pass: it takes a snapshot of the
 * presses and clears them, so isButtonPressed() answers for the current pass
 * only. A press that the current mode does not use is simply dropped and can
 * never be acted on later by another mode.
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

// Call once before starting TIM2
void button_init(void);

// Called from the TIM2 interrupt every TIMER_CYCLE ms
void button_reading(void);

// Called once at the start of every main-loop pass
void button_collect_events(void);

// 1 if the button was pressed (or auto-repeated) since the previous pass
int isButtonPressed(button_id_t id);

#endif /* INC_BUTTON_H_ */
