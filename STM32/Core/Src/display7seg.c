/*
 * display7seg.c
 *
 * 7-segment multiplexing driver
 */

#include "display7seg.h"

typedef struct {
    GPIO_TypeDef *port;
    uint16_t pin;
} out_pin_t;

static const out_pin_t segPins[7] = {
    {SEG_A_GPIO_Port, SEG_A_Pin}, {SEG_B_GPIO_Port, SEG_B_Pin},
    {SEG_C_GPIO_Port, SEG_C_Pin}, {SEG_D_GPIO_Port, SEG_D_Pin},
    {SEG_E_GPIO_Port, SEG_E_Pin}, {SEG_F_GPIO_Port, SEG_F_Pin},
    {SEG_G_GPIO_Port, SEG_G_Pin},
};

static const out_pin_t enPins[NUM_OF_DIGITS] = {
    {EN0_GPIO_Port, EN0_Pin}, {EN1_GPIO_Port, EN1_Pin},
    {EN2_GPIO_Port, EN2_Pin}, {EN3_GPIO_Port, EN3_Pin},
};

// Common anode: bit = 1 means the segment is OFF. Bit 0 = A ... bit 6 = G.
static const uint8_t segmentPatterns[10] = {
    0x40, 0x79, 0x24, 0x30, 0x19, 0x12, 0x02, 0x78, 0x00, 0x10
};

static int ledBuffer[NUM_OF_DIGITS] = {0, 0, 0, 0};
static int scanIndex = 0;

static void disableAllDigits(void) {
    for (int i = 0; i < NUM_OF_DIGITS; i++) {
        HAL_GPIO_WritePin(enPins[i].port, enPins[i].pin, DISABLE_7SEG);
    }
}

static void writeSegments(int num) {
    uint8_t pattern = (num >= 0 && num <= 9) ? segmentPatterns[num] : 0x7F; // 0x7F = blank
    for (int s = 0; s < 7; s++) {
        HAL_GPIO_WritePin(segPins[s].port, segPins[s].pin,
                          ((pattern >> s) & 1) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    }
}

void display_init(void) {
    disableAllDigits();
    writeSegments(-1);
    scanIndex = 0;
}

static int clamp99(int value) {
    if (value < 0) return 0;
    if (value > 99) return 99;
    return value;
}

void display_setNumbers(int left, int right) {
    left = clamp99(left);
    right = clamp99(right);
    ledBuffer[0] = left / 10;
    ledBuffer[1] = left % 10;
    ledBuffer[2] = right / 10;
    ledBuffer[3] = right % 10;
}

void display_scan(void) {
    // Blank first so the old digit never shows the new segment pattern (ghosting)
    disableAllDigits();
    writeSegments(ledBuffer[scanIndex]);
    HAL_GPIO_WritePin(enPins[scanIndex].port, enPins[scanIndex].pin, ENABLE_7SEG);
    scanIndex = (scanIndex + 1) % NUM_OF_DIGITS;
}
