/*
 * led_display.h
 *
 *  Created on: Nov 2, 2026
 *      Author: pham0
 */

#ifndef INC_LED_DISPLAY_H_
#define INC_LED_DISPLAY_H_

#include "main.h"

void display7SEG(int num);
void update7SEG(int index);
void update7SEG_Buffer(int mode, int value);
void turnOffAllLeds(void);
void toggleRedLeds(void);
void toggleAmberLeds(void);
void toggleGreenLeds(void);
void setTrafficLeds(uint8_t road1_color, uint8_t road2_color);

#endif /* INC_LED_DISPLAY_H_ */
