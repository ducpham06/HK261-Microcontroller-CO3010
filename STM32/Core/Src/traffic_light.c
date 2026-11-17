/*
 * traffic_light.c
 */

#include "traffic_light.h"
#include "global.h"

void setLights(GPIO_PinState red1, GPIO_PinState amber1, GPIO_PinState green1,
               GPIO_PinState red2, GPIO_PinState amber2, GPIO_PinState green2) {
    HAL_GPIO_WritePin(RED1_GPIO_Port, RED1_Pin, red1);
    HAL_GPIO_WritePin(AMBER1_GPIO_Port, AMBER1_Pin, amber1);
    HAL_GPIO_WritePin(GREEN1_GPIO_Port, GREEN1_Pin, green1);
    HAL_GPIO_WritePin(RED2_GPIO_Port, RED2_Pin, red2);
    HAL_GPIO_WritePin(AMBER2_GPIO_Port, AMBER2_Pin, amber2);
    HAL_GPIO_WritePin(GREEN2_GPIO_Port, GREEN2_Pin, green2);
}

void setPhaseLights(int phase) {
    switch (phase) {
        case R_G: setLights(ON, OFF, OFF,  OFF, OFF, ON);  break;
        case R_A: setLights(ON, OFF, OFF,  OFF, ON,  OFF); break;
        case G_R: setLights(OFF, OFF, ON,  ON,  OFF, OFF); break;
        case A_R: setLights(OFF, ON,  OFF, ON,  OFF, OFF); break;
        default:  setLights(OFF, OFF, OFF, OFF, OFF, OFF); break;
    }
}

/* ---------------- 7-segment display ---------------- */

static GPIO_TypeDef *const segPort[7] = {
    SEG_A_GPIO_Port, SEG_B_GPIO_Port, SEG_C_GPIO_Port, SEG_D_GPIO_Port,
    SEG_E_GPIO_Port, SEG_F_GPIO_Port, SEG_G_GPIO_Port
};
static const uint16_t segPin[7] = {
    SEG_A_Pin, SEG_B_Pin, SEG_C_Pin, SEG_D_Pin, SEG_E_Pin, SEG_F_Pin, SEG_G_Pin
};
static GPIO_TypeDef *const enPort[4] = {EN0_GPIO_Port, EN1_GPIO_Port, EN2_GPIO_Port, EN3_GPIO_Port};
static const uint16_t enPin[4] = {EN0_Pin, EN1_Pin, EN2_Pin, EN3_Pin};

// Bit 0 = segment A ... bit 6 = segment G, 1 = segment off
static const uint8_t digitPattern[10] = {0x40, 0x79, 0x24, 0x30, 0x19, 0x12, 0x02, 0x78, 0x00, 0x10};

static int ledBuffer[4];
static int scanIndex = 0;

void updateLedBuffer(int left, int right) {
    ledBuffer[0] = left / 10;
    ledBuffer[1] = left % 10;
    ledBuffer[2] = right / 10;
    ledBuffer[3] = right % 10;
}

void display_scan(void) {
    // Turn every digit off first so the new pattern never shows on the old digit
    for (int i = 0; i < 4; i++) {
        HAL_GPIO_WritePin(enPort[i], enPin[i], OFF);
    }
    for (int s = 0; s < 7; s++) {
        HAL_GPIO_WritePin(segPort[s], segPin[s], (GPIO_PinState)((digitPattern[ledBuffer[scanIndex]] >> s) & 1));
    }
    HAL_GPIO_WritePin(enPort[scanIndex], enPin[scanIndex], ON);
    scanIndex = (scanIndex + 1) % 4;
}
