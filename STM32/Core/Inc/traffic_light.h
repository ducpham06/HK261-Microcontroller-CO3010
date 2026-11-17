/*
 * traffic_light.h
 *
 * LEDs (PA1-PA6) and 7-segment digits (segments PB3-PB9, enables PA7-PA10).
 * Everything is active-low.
 *
 * Each group of pins is changed with one BSRR write, so all its pins switch
 * at the same instant. Proteus then recalculates the LED/transistor circuit
 * once per write instead of once per pin, which keeps the simulation fast.
 */

#ifndef INC_TRAFFIC_LIGHT_H_
#define INC_TRAFFIC_LIGHT_H_

#include "main.h"

// How long each digit stays on. Lower = steadier display, higher = faster
// Proteus simulation. Must be a multiple of 10 ms (the timer tick).
#define SCAN_PERIOD_MS  20

// Turn on exactly these LEDs, e.g. setLights(RED1_Pin | GREEN2_Pin); 0 = all off
void setLights(uint16_t onPins);
void setPhaseLights(int phase);

// Left number on EN0-EN1, right number on EN2-EN3 (0-99 each)
void updateLedBuffer(int left, int right);

// Show the next digit; call every SCAN_PERIOD_MS
void display_scan(void);

#endif /* INC_TRAFFIC_LIGHT_H_ */
