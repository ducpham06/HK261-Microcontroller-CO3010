#include "main.h"
#include "button.h"
#include "software_timer.h"

#define PRESSED         GPIO_PIN_RESET
#define RELEASED        GPIO_PIN_SET
#define TICKS_1S        (1000 / TIMER_CYCLE)
#define TICKS_500MS     (500 / TIMER_CYCLE)

static GPIO_TypeDef *const buttonPort[NO_OF_BUTTONS] = {BUTTON_1_GPIO_Port, BUTTON_2_GPIO_Port, BUTTON_3_GPIO_Port};
static const uint16_t buttonPin[NO_OF_BUTTONS] = {BUTTON_1_Pin, BUTTON_2_Pin, BUTTON_3_Pin};

static GPIO_PinState keyReg0[NO_OF_BUTTONS] = {RELEASED, RELEASED, RELEASED};
static GPIO_PinState keyReg1[NO_OF_BUTTONS] = {RELEASED, RELEASED, RELEASED};
static GPIO_PinState keyReg2[NO_OF_BUTTONS] = {RELEASED, RELEASED, RELEASED};
static GPIO_PinState stableState[NO_OF_BUTTONS] = {RELEASED, RELEASED, RELEASED};
static int holdCounter[NO_OF_BUTTONS];
static volatile int pressFlag[NO_OF_BUTTONS];

void getKeyInput(void) {
    for (int i = 0; i < NO_OF_BUTTONS; i++) {
        pressFlag[i] = 0;

        keyReg0[i] = keyReg1[i];
        keyReg1[i] = keyReg2[i];
        keyReg2[i] = HAL_GPIO_ReadPin(buttonPort[i], buttonPin[i]);
        if (keyReg0[i] != keyReg1[i] || keyReg1[i] != keyReg2[i]) continue;   // Bouncing

        GPIO_PinState lastState = stableState[i];
        stableState[i] = keyReg2[i];
        if (keyReg2[i] == RELEASED) continue;

        if (lastState == RELEASED) {                 // New press
            pressFlag[i] = 1;
            holdCounter[i] = TICKS_1S;
            continue;
        }

        if (i != BUTTON_INC) continue;               // Only Button 2 repeats
        holdCounter[i]--;
        if (holdCounter[i] > 0) continue;
        pressFlag[i] = 1;                            // Repeat while held
        holdCounter[i] = TICKS_500MS;
    }
}

int isButtonPressed(int index) {
    if (pressFlag[index] == 0) return 0;
    pressFlag[index] = 0;
    return 1;
}
