/*
 * traffic_light.h
 *
 * LEDs and 7-segment digits. Everything is active-low.
 */

#ifndef INC_TRAFFIC_LIGHT_H_
#define INC_TRAFFIC_LIGHT_H_

#include "main.h"

#define ON      GPIO_PIN_RESET
#define OFF     GPIO_PIN_SET

void setLights(GPIO_PinState red1, GPIO_PinState amber1, GPIO_PinState green1,
               GPIO_PinState red2, GPIO_PinState amber2, GPIO_PinState green2);
void setPhaseLights(int phase);

// Left number on EN0-EN1, right number on EN2-EN3 (0-99 each)
void updateLedBuffer(int left, int right);

// Show the next digit; call every 10 ms
void display_scan(void);

#endif /* INC_TRAFFIC_LIGHT_H_ */
