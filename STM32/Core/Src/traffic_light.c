/*
 * traffic_light.c
 *
 * BSRR: writing 1 to bit n sets pin n high, writing 1 to bit n+16 sets it low.
 * All pins named in one write change together.
 */

#include "traffic_light.h"
#include "global.h"

#define ALL_LIGHTS  (RED1_Pin | AMBER1_Pin | GREEN1_Pin | RED2_Pin | AMBER2_Pin | GREEN2_Pin)
#define ALL_EN      (EN0_Pin | EN1_Pin | EN2_Pin | EN3_Pin)
#define ALL_SEG     (SEG_A_Pin | SEG_B_Pin | SEG_C_Pin | SEG_D_Pin | SEG_E_Pin | SEG_F_Pin | SEG_G_Pin)

// Active-low: pins to turn on go low (upper half), the others go high (lower half)
#define ACTIVE_LOW(onPins, allPins)  (((uint32_t)(onPins) << 16) | ((allPins) & ~(onPins)))

void setLights(uint16_t onPins) {
    GPIOA->BSRR = ACTIVE_LOW(onPins, ALL_LIGHTS);
}

void setPhaseLights(int phase) {
    switch (phase) {
        case R_G: setLights(RED1_Pin | GREEN2_Pin);   break;
        case R_A: setLights(RED1_Pin | AMBER2_Pin);   break;
        case G_R: setLights(GREEN1_Pin | RED2_Pin);   break;
        case A_R: setLights(AMBER1_Pin | RED2_Pin);   break;
        default:  setLights(0);                       break;
    }
}

/* ---------------- 7-segment display ---------------- */

// Segments lit for each digit
static const uint16_t digitSegments[10] = {
    SEG_A_Pin | SEG_B_Pin | SEG_C_Pin | SEG_D_Pin | SEG_E_Pin | SEG_F_Pin,              // 0
    SEG_B_Pin | SEG_C_Pin,                                                              // 1
    SEG_A_Pin | SEG_B_Pin | SEG_D_Pin | SEG_E_Pin | SEG_G_Pin,                          // 2
    SEG_A_Pin | SEG_B_Pin | SEG_C_Pin | SEG_D_Pin | SEG_G_Pin,                          // 3
    SEG_B_Pin | SEG_C_Pin | SEG_F_Pin | SEG_G_Pin,                                      // 4
    SEG_A_Pin | SEG_C_Pin | SEG_D_Pin | SEG_F_Pin | SEG_G_Pin,                          // 5
    SEG_A_Pin | SEG_C_Pin | SEG_D_Pin | SEG_E_Pin | SEG_F_Pin | SEG_G_Pin,              // 6
    SEG_A_Pin | SEG_B_Pin | SEG_C_Pin,                                                  // 7
    SEG_A_Pin | SEG_B_Pin | SEG_C_Pin | SEG_D_Pin | SEG_E_Pin | SEG_F_Pin | SEG_G_Pin,  // 8
    SEG_A_Pin | SEG_B_Pin | SEG_C_Pin | SEG_D_Pin | SEG_F_Pin | SEG_G_Pin,              // 9
};

static const uint16_t enPin[4] = {EN0_Pin, EN1_Pin, EN2_Pin, EN3_Pin};

static int ledBuffer[4];
static int scanIndex = 0;

void updateLedBuffer(int left, int right) {
    ledBuffer[0] = left / 10;
    ledBuffer[1] = left % 10;
    ledBuffer[2] = right / 10;
    ledBuffer[3] = right % 10;
}

void display_scan(void) {
    GPIOA->BSRR = ALL_EN;                                               // All digits off
    GPIOB->BSRR = ACTIVE_LOW(digitSegments[ledBuffer[scanIndex]], ALL_SEG);
    GPIOA->BSRR = (uint32_t)enPin[scanIndex] << 16;                     // This digit on
    scanIndex = (scanIndex + 1) % 4;
}
