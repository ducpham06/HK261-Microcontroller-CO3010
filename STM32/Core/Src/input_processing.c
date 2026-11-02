/*
 * input_processing.c
 *
 *  Created on: Nov 2, 2026
 *      Author: pham0
 */


#include "input_processing.h"
#include "input_reading.h"
#include "led_display.h"
#include "timer.h"

enum SystemMode { MODE_NORMAL = 1, MODE_MODIFY_RED, MODE_MODIFY_AMBER, MODE_MODIFY_GREEN };
enum TrafficState { R1_RED_R2_GREEN, R1_RED_R2_AMBER, R1_GREEN_R2_RED, R1_AMBER_R2_RED };

static enum SystemMode currentMode = MODE_NORMAL;
static enum TrafficState trafficState = R1_RED_R2_GREEN;

// Default initial durations (in seconds)
static int redDuration = 5;
static int amberDuration = 2;
static int greenDuration = 3;

// Temporary editing buffers for Modes 2, 3, and 4
static int tempDuration = 0;

// Countdown counters for normal mode
static int countRoad1 = 5;
static int countRoad2 = 3;
static int current_seg_idx = 0;

void fsm_for_input_processing(void) {
    // 1. Multiplex 7-segment displays every 5ms
    if (timer3_flag == 1) {
        setTimer3(5);
        update7SEG(current_seg_idx);
        current_seg_idx = (current_seg_idx + 1) % 4;
    }

    // 2. Global Button 1 handling: Cycle through Modes 1 -> 2 -> 3 -> 4 -> 1
    if (is_button_pressed(0)) {
        if (currentMode == MODE_NORMAL) {
            currentMode = MODE_MODIFY_RED;
            tempDuration = redDuration;
            turnOffAllLeds();
            setTimer2(250); // 250ms half-period -> 2 Hz toggle
        } else if (currentMode == MODE_MODIFY_RED) {
            currentMode = MODE_MODIFY_AMBER;
            tempDuration = amberDuration;
            turnOffAllLeds();
            setTimer2(250);
        } else if (currentMode == MODE_MODIFY_AMBER) {
            currentMode = MODE_MODIFY_GREEN;
            tempDuration = greenDuration;
            turnOffAllLeds();
            setTimer2(250);
        } else if (currentMode == MODE_MODIFY_GREEN) {
            currentMode = MODE_NORMAL;
            trafficState = R1_RED_R2_GREEN;
            countRoad1 = redDuration;
            countRoad2 = greenDuration;
            setTimer1(1000);
        }
    }

    // 3. FSM Mode Processing
    switch (currentMode) {
        case MODE_NORMAL:
            update7SEG_Buffer(countRoad1, countRoad2);

            // Set appropriate traffic light signals
            switch (trafficState) {
                case R1_RED_R2_GREEN: setTrafficLeds(0, 2); break;
                case R1_RED_R2_AMBER: setTrafficLeds(0, 1); break;
                case R1_GREEN_R2_RED: setTrafficLeds(2, 0); break;
                case R1_AMBER_R2_RED: setTrafficLeds(1, 0); break;
            }

            // 1-second countdown cycle
            if (timer1_flag == 1) {
                setTimer1(1000);
                countRoad1--;
                countRoad2--;

                if (trafficState == R1_RED_R2_GREEN && countRoad2 <= 0) {
                    trafficState = R1_RED_R2_AMBER;
                    countRoad2 = amberDuration;
                } else if (trafficState == R1_RED_R2_AMBER && countRoad1 <= 0) {
                    trafficState = R1_GREEN_R2_RED;
                    countRoad1 = greenDuration;
                    countRoad2 = redDuration;
                } else if (trafficState == R1_GREEN_R2_RED && countRoad1 <= 0) {
                    trafficState = R1_AMBER_R2_RED;
                    countRoad1 = amberDuration;
                } else if (trafficState == R1_AMBER_R2_RED && countRoad2 <= 0) {
                    trafficState = R1_RED_R2_GREEN;
                    countRoad1 = redDuration;
                    countRoad2 = greenDuration;
                }
            }
            break;

        case MODE_MODIFY_RED:
            update7SEG_Buffer(tempDuration, 2); // Show value and mode 02
            if (timer2_flag == 1) {
                setTimer2(250);
                toggleRedLeds();
            }
            if (is_button_pressed(1)) { // Increase value (1-99)
                tempDuration++;
                if (tempDuration > 99) tempDuration = 1;
            }
            if (is_button_pressed(2)) { // Set value
                redDuration = tempDuration;
            }
            break;

        case MODE_MODIFY_AMBER:
            update7SEG_Buffer(tempDuration, 3); // Show value and mode 03
            if (timer2_flag == 1) {
                setTimer2(250);
                toggleAmberLeds();
            }
            if (is_button_pressed(1)) {
                tempDuration++;
                if (tempDuration > 99) tempDuration = 1;
            }
            if (is_button_pressed(2)) {
                amberDuration = tempDuration;
            }
            break;

        case MODE_MODIFY_GREEN:
            update7SEG_Buffer(tempDuration, 4); // Show value and mode 04
            if (timer2_flag == 1) {
                setTimer2(250);
                toggleGreenLeds();
            }
            if (is_button_pressed(1)) {
                tempDuration++;
                if (tempDuration > 99) tempDuration = 1;
            }
            if (is_button_pressed(2)) {
                greenDuration = tempDuration;
            }
            break;
    }
}
