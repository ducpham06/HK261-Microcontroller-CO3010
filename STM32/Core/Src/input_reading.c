/*
 * input_reading.c
 *
 *  Created on: Nov 2, 2026
 *      Author: pham0
 */


#include "input_reading.h"

#define DURATION_FOR_AUTO_INCREASING 100 // 100 * 10ms = 1 second
#define BUTTON_IS_PRESSED            GPIO_PIN_RESET
#define BUTTON_IS_RELEASED           GPIO_PIN_SET

static GPIO_PinState buttonBuffer[NO_OF_BUTTONS] = {BUTTON_IS_RELEASED, BUTTON_IS_RELEASED, BUTTON_IS_RELEASED};
static GPIO_PinState debounceButtonBuffer1[NO_OF_BUTTONS] = {BUTTON_IS_RELEASED, BUTTON_IS_RELEASED, BUTTON_IS_RELEASED};
static GPIO_PinState debounceButtonBuffer2[NO_OF_BUTTONS] = {BUTTON_IS_RELEASED, BUTTON_IS_RELEASED, BUTTON_IS_RELEASED};

static uint8_t flagForButtonPress1s[NO_OF_BUTTONS] = {0};
static uint16_t counterForButtonPress1s[NO_OF_BUTTONS] = {0};
static uint8_t button_pressed_flag[NO_OF_BUTTONS] = {0};

void button_reading(void) {
    for (uint8_t i = 0; i < NO_OF_BUTTONS; i++) {
        debounceButtonBuffer2[i] = debounceButtonBuffer1[i];

        if (i == 0) {
            debounceButtonBuffer1[i] = HAL_GPIO_ReadPin(BUTTON_1_GPIO_Port, BUTTON_1_Pin);
        } else if (i == 1) {
            debounceButtonBuffer1[i] = HAL_GPIO_ReadPin(BUTTON_2_GPIO_Port, BUTTON_2_Pin);
        } else if (i == 2) {
            debounceButtonBuffer1[i] = HAL_GPIO_ReadPin(BUTTON_3_GPIO_Port, BUTTON_3_Pin);
        }

        if (debounceButtonBuffer1[i] == debounceButtonBuffer2[i]) {
            if (buttonBuffer[i] != debounceButtonBuffer1[i]) {
                buttonBuffer[i] = debounceButtonBuffer1[i];
                if (buttonBuffer[i] == BUTTON_IS_PRESSED) {
                    button_pressed_flag[i] = 1;
                }
            }

            if (buttonBuffer[i] == BUTTON_IS_PRESSED) {
                if (counterForButtonPress1s[i] < DURATION_FOR_AUTO_INCREASING) {
                    counterForButtonPress1s[i]++;
                } else {
                    flagForButtonPress1s[i] = 1;
                    counterForButtonPress1s[i] = DURATION_FOR_AUTO_INCREASING - 50;
                    button_pressed_flag[i] = 1;
                }
            } else {
                counterForButtonPress1s[i] = 0;
                flagForButtonPress1s[i] = 0;
            }
        }
    }
}

unsigned char is_button_pressed(unsigned char index) {
    if (index >= NO_OF_BUTTONS) return 0;
    if (button_pressed_flag[index] == 1) {
        button_pressed_flag[index] = 0; // Consume single-click event
        return 1;
    }
    return 0;
}

unsigned char is_button_pressed_1s(unsigned char index) {
    if (index >= NO_OF_BUTTONS) return 0;
    return (flagForButtonPress1s[index] == 1);
}
