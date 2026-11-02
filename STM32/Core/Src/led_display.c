/*
 * led_display.c
 *
 *  Created on: Nov 2, 2026
 *      Author: pham0
 */


#include "led_display.h"

// 7-segment common-anode conversion table (0 to 9)
const uint8_t segment_pattern[10] = {
    0x3F, // 0
    0x06, // 1
    0x5B, // 2
    0x4F, // 3
    0x66, // 4
    0x6D, // 5
    0x7D, // 6
    0x07, // 7
    0x7F, // 8
    0x6F  // 9
};

// 4 multiplexed digit buffers: [Road 1 Tens, Road 1 Units, Road 2 Tens, Road 2 Units]
int seg_buffer[4] = {0, 1, 0, 0};

void display7SEG(int num) {
    if (num < 0 || num > 9) return;
    uint8_t pattern = segment_pattern[num];

    // Output to individual segment lines (a-g on SEG_A to SEG_G pins, active LOW)
    HAL_GPIO_WritePin(SEG_A_GPIO_Port, SEG_A_Pin, (pattern & 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(SEG_B_GPIO_Port, SEG_B_Pin, (pattern & 0x02) ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(SEG_C_GPIO_Port, SEG_C_Pin, (pattern & 0x04) ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(SEG_D_GPIO_Port, SEG_D_Pin, (pattern & 0x08) ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(SEG_E_GPIO_Port, SEG_E_Pin, (pattern & 0x10) ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(SEG_F_GPIO_Port, SEG_F_Pin, (pattern & 0x20) ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(SEG_G_GPIO_Port, SEG_G_Pin, (pattern & 0x40) ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

void update7SEG_Buffer(int val1, int val2) {
    seg_buffer[0] = val1 / 10;
    seg_buffer[1] = val1 % 10;
    seg_buffer[2] = val2 / 10;
    seg_buffer[3] = val2 % 10;
}

void update7SEG(int index) {
    // Disable all 4 digits (assuming active LOW enable pins)
    HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, GPIO_PIN_SET);

    display7SEG(seg_buffer[index]);

    switch (index) {
        case 0: HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, GPIO_PIN_RESET); break;
        case 1: HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, GPIO_PIN_RESET); break;
        case 2: HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, GPIO_PIN_RESET); break;
        case 3: HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, GPIO_PIN_RESET); break;
        default: break;
    }
}

void turnOffAllLeds(void) {
    HAL_GPIO_WritePin(RED1_GPIO_Port, RED1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(AMBER1_GPIO_Port, AMBER1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GREEN1_GPIO_Port, GREEN1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(RED2_GPIO_Port, RED2_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(AMBER2_GPIO_Port, AMBER2_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GREEN2_GPIO_Port, GREEN2_Pin, GPIO_PIN_RESET);
}

void toggleRedLeds(void) {
    HAL_GPIO_TogglePin(RED1_GPIO_Port, RED1_Pin);
    HAL_GPIO_TogglePin(RED2_GPIO_Port, RED2_Pin);
}

void toggleAmberLeds(void) {
    HAL_GPIO_TogglePin(AMBER1_GPIO_Port, AMBER1_Pin);
    HAL_GPIO_TogglePin(AMBER2_GPIO_Port, AMBER2_Pin);
}

void toggleGreenLeds(void) {
    HAL_GPIO_TogglePin(GREEN1_GPIO_Port, GREEN1_Pin);
    HAL_GPIO_TogglePin(GREEN2_GPIO_Port, GREEN2_Pin);
}

void setTrafficLeds(uint8_t road1_color, uint8_t road2_color) {
    turnOffAllLeds();
    // 0: RED, 1: AMBER, 2: GREEN
    if (road1_color == 0) HAL_GPIO_WritePin(RED1_GPIO_Port, RED1_Pin, GPIO_PIN_SET);
    else if (road1_color == 1) HAL_GPIO_WritePin(AMBER1_GPIO_Port, AMBER1_Pin, GPIO_PIN_SET);
    else if (road1_color == 2) HAL_GPIO_WritePin(GREEN1_GPIO_Port, GREEN1_Pin, GPIO_PIN_SET);

    if (road2_color == 0) HAL_GPIO_WritePin(RED2_GPIO_Port, RED2_Pin, GPIO_PIN_SET);
    else if (road2_color == 1) HAL_GPIO_WritePin(AMBER2_GPIO_Port, AMBER2_Pin, GPIO_PIN_SET);
    else if (road2_color == 2) HAL_GPIO_WritePin(GREEN2_GPIO_Port, GREEN2_Pin, GPIO_PIN_SET);
}
