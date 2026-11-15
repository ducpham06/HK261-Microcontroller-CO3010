/*
 * display7seg.h
 *
 * Four multiplexed common-anode 7-segment digits.
 * Segments are on PB3-PB9 (active-low); EN0-EN3 drive PNP transistors
 * (active-low). Digits EN0-EN1 form the left number, EN2-EN3 the right one.
 */

#ifndef INC_DISPLAY7SEG_H_
#define INC_DISPLAY7SEG_H_

#include "main.h"

#define ENABLE_7SEG     GPIO_PIN_RESET
#define DISABLE_7SEG    GPIO_PIN_SET

#define NUM_OF_DIGITS   4
#define SCAN_PERIOD_MS  10      // Time each digit stays on (25 Hz full refresh)

// Blank the display and reset the scan position
void display_init(void);

// Put two 2-digit numbers (0-99) into the display buffer
void display_setNumbers(int left, int right);

// Show the next digit; call every SCAN_PERIOD_MS
void display_scan(void);

#endif /* INC_DISPLAY7SEG_H_ */
