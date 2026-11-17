#include "fsm_setting.h"
#include "global.h"
#include "button.h"
#include "software_timer.h"
#include "traffic_light.h"

void fsm_setting(void) {
    if (mode != MODE_RED && mode != MODE_AMBER && mode != MODE_GREEN) return;

    updateLedBuffer(mode, edit_value);

    // Button 1: next mode
    if (isButtonPressed(BUTTON_MODE)) {
        switch (mode) {
            case MODE_RED:
                mode = MODE_AMBER;
                edit_value = time_amber;
                blink_on = 1;
                setLights(AMBER1_Pin | AMBER2_Pin);
                setTimer(TIMER_BLINK, BLINK_TOGGLE_MS);
                break;
            case MODE_AMBER:
                mode = MODE_GREEN;
                edit_value = time_green;
                blink_on = 1;
                setLights(GREEN1_Pin | GREEN2_Pin);
                setTimer(TIMER_BLINK, BLINK_TOGGLE_MS);
                break;
            case MODE_GREEN:
                mode = MODE_NORMAL;
                phase = R_G;
                countdown1 = time_red;
                countdown2 = time_green;
                setLights(RED1_Pin | GREEN2_Pin);
                setTimer(TIMER_COUNTDOWN, 1000);
                break;
        }
        return;
    }

    // Blink the LEDs of the selected color
    if (isTimerExpired(TIMER_BLINK)) {
        setTimer(TIMER_BLINK, BLINK_TOGGLE_MS);
        blink_on = !blink_on;
        uint16_t pins = 0;
        switch (mode) {
            case MODE_RED:   pins = RED1_Pin | RED2_Pin;     break;
            case MODE_AMBER: pins = AMBER1_Pin | AMBER2_Pin; break;
            case MODE_GREEN: pins = GREEN1_Pin | GREEN2_Pin; break;
        }
        setLights(pins * blink_on);                 // blink_on = 0 turns them all off
    }

    // Button 2: increase the value
    if (isButtonPressed(BUTTON_INC)) edit_value = edit_value % 99 + 1;

    // Button 3: save the value
    if (!isButtonPressed(BUTTON_SET)) return;

    int red = time_red;
    int amber = time_amber;
    int green = time_green;
    switch (mode) {
        case MODE_RED:   red = edit_value;   green = red - amber; break;
        case MODE_AMBER: amber = edit_value; green = red - amber; break;
        case MODE_GREEN: green = edit_value; red = amber + green; break;
    }

    if (red <= 99 && amber >= 1 && green >= 1) {
        time_red = red;
        time_amber = amber;
        time_green = green;
    }

    // Show the stored value (the new one, or the old one if refused)
    switch (mode) {
        case MODE_RED:   edit_value = time_red;   break;
        case MODE_AMBER: edit_value = time_amber; break;
        case MODE_GREEN: edit_value = time_green; break;
    }
}
