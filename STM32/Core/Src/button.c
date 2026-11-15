/*
 * button.c
 *
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

// Written by the interrupt, consumed by button_collect_events()
static volatile uint8_t pressFlag[NO_OF_BUTTONS];
// Snapshot used by the FSMs during one main-loop pass
static uint8_t pressEvent[NO_OF_BUTTONS];

static GPIO_PinState readButton(int index) {
    switch (index) {
        case BUTTON_MODE: return HAL_GPIO_ReadPin(BUTTON_1_GPIO_Port, BUTTON_1_Pin);
        case BUTTON_INC:  return HAL_GPIO_ReadPin(BUTTON_2_GPIO_Port, BUTTON_2_Pin);
        case BUTTON_SET:  return HAL_GPIO_ReadPin(BUTTON_3_GPIO_Port, BUTTON_3_Pin);
        default:          return BUTTON_IS_RELEASED;
    }
}

void button_reading(void) {
    for (int i = 0; i < NO_OF_BUTTONS; i++) {
        keyReg0[i] = keyReg1[i];
        keyReg1[i] = keyReg2[i];
        keyReg2[i] = readButton(i);

        // Still bouncing: wait for three equal samples
        if (keyReg0[i] != keyReg1[i] || keyReg1[i] != keyReg2[i]) {
            continue;
        }

        if (keyReg2[i] == BUTTON_IS_PRESSED) {
            if (stableState[i] == BUTTON_IS_RELEASED) {
                pressFlag[i] = 1;                  // New press
                holdCounter[i] = TICKS_FOR_1S;     // First repeat after 1 s
            } else if (allowAutoRepeat[i]) {
                if (--holdCounter[i] == 0) {
                    pressFlag[i] = 1;              // Repeat while held
                    holdCounter[i] = TICKS_FOR_AUTO_REPEAT;
                }
            }
        }
        stableState[i] = keyReg2[i];
    }
}

void button_collect_events(void) {
    for (int i = 0; i < NO_OF_BUTTONS; i++) {
        pressEvent[i] = pressFlag[i];
        if (pressEvent[i]) {
            pressFlag[i] = 0;
        }
    }
}

int isButtonPressed(button_id_t id) {
    if (id >= NO_OF_BUTTONS) return 0;
    return pressEvent[id];
}

void button_init(void) {
    // Pins are pulled up, so every button starts released
    for (int i = 0; i < NO_OF_BUTTONS; i++) {
        keyReg0[i] = keyReg1[i] = keyReg2[i] = stableState[i] = BUTTON_IS_RELEASED;
        holdCounter[i] = 0;
        pressFlag[i] = 0;
        pressEvent[i] = 0;
    }
}
