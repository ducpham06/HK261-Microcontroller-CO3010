/*
 * button.c
 *
 * Runs as a scheduler task (getKeyInput) every TIMER_CYCLE ms.
 * Debounce: a new level is accepted only when three consecutive samples
 * (3 x TIMER_CYCLE = 30 ms) agree. A press event is raised on the
 * released -> pressed edge. Button 2 auto-repeats while held: the first
 * repeat comes after 1 s, then every 500 ms (as in the lab's button example).
 */

#include "button.h"
#include "software_timer.h"

#define TICKS_FOR_1S            (1000 / TIMER_CYCLE)
#define TICKS_FOR_AUTO_REPEAT   (500 / TIMER_CYCLE)

// Only the "increase" button repeats; repeating MODE or SET would skip states
static const uint8_t allowAutoRepeat[NO_OF_BUTTONS] = {0, 1, 0};

static GPIO_PinState keyReg0[NO_OF_BUTTONS];
static GPIO_PinState keyReg1[NO_OF_BUTTONS];
static GPIO_PinState keyReg2[NO_OF_BUTTONS];
static GPIO_PinState stableState[NO_OF_BUTTONS];
static uint16_t holdCounter[NO_OF_BUTTONS];

// Presses found in the current tick, cleared when read
static uint8_t pressEvent[NO_OF_BUTTONS];

static GPIO_PinState readButton(int index) {
    switch (index) {
        case BUTTON_MODE: return HAL_GPIO_ReadPin(BUTTON_1_GPIO_Port, BUTTON_1_Pin);
        case BUTTON_INC:  return HAL_GPIO_ReadPin(BUTTON_2_GPIO_Port, BUTTON_2_Pin);
        case BUTTON_SET:  return HAL_GPIO_ReadPin(BUTTON_3_GPIO_Port, BUTTON_3_Pin);
        default:          return BUTTON_IS_RELEASED;
    }
}

void getKeyInput(void) {
    for (int i = 0; i < NO_OF_BUTTONS; i++) {
        pressEvent[i] = 0;      // Drop presses no FSM used in the previous tick

        keyReg0[i] = keyReg1[i];
        keyReg1[i] = keyReg2[i];
        keyReg2[i] = readButton(i);

        // Still bouncing: wait for three equal samples
        if (keyReg0[i] != keyReg1[i] || keyReg1[i] != keyReg2[i]) {
            continue;
        }

        if (keyReg2[i] == BUTTON_IS_PRESSED) {
            if (stableState[i] == BUTTON_IS_RELEASED) {
                pressEvent[i] = 1;                 // New press
                holdCounter[i] = TICKS_FOR_1S;     // First repeat after 1 s
            } else if (allowAutoRepeat[i]) {
                if (--holdCounter[i] == 0) {
                    pressEvent[i] = 1;             // Repeat while held
                    holdCounter[i] = TICKS_FOR_AUTO_REPEAT;
                }
            }
        }
        stableState[i] = keyReg2[i];
    }
}

int isButtonPressed(button_id_t id) {
    if (id >= NO_OF_BUTTONS || pressEvent[id] == 0) return 0;
    pressEvent[id] = 0;
    return 1;
}

void button_init(void) {
    // Pins are pulled up, so every button starts released
    for (int i = 0; i < NO_OF_BUTTONS; i++) {
        keyReg0[i] = keyReg1[i] = keyReg2[i] = stableState[i] = BUTTON_IS_RELEASED;
        holdCounter[i] = 0;
        pressEvent[i] = 0;
    }
}
