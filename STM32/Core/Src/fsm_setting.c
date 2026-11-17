#include "fsm_setting.h"
#include "global.h"
#include "button.h"
#include "software_timer.h"
#include "traffic_light.h"

void fsm_setting(void) {
    switch (status) {
        case SET_RED:
            updateLedBuffer(2, edit_value);
            if (isButtonPressed(BUTTON_MODE)) {
                status = SET_AMBER;             // Unsaved value is dropped
                edit_value = time_amber;
                blink_on = 1;
                setLights(AMBER1_Pin | AMBER2_Pin);
                setTimer(TIMER_BLINK, BLINK_TOGGLE_MS);
                break;
            }
            if (isTimerExpired(TIMER_BLINK)) {
                setTimer(TIMER_BLINK, BLINK_TOGGLE_MS);
                blink_on = !blink_on;
                setLights((RED1_Pin | RED2_Pin) * blink_on);
            }
            if (isButtonPressed(BUTTON_INC)) edit_value = edit_value % 99 + 1;
            if (!isButtonPressed(BUTTON_SET)) break;
            // Save: green = red - amber must stay >= 1, otherwise refused
            if (edit_value > time_amber) {
                time_red = edit_value;
                time_green = time_red - time_amber;
            }
            edit_value = time_red;
            break;

        case SET_AMBER:
            updateLedBuffer(3, edit_value);
            if (isButtonPressed(BUTTON_MODE)) {
                status = SET_GREEN;
                edit_value = time_green;
                blink_on = 1;
                setLights(GREEN1_Pin | GREEN2_Pin);
                setTimer(TIMER_BLINK, BLINK_TOGGLE_MS);
                break;
            }
            if (isTimerExpired(TIMER_BLINK)) {
                setTimer(TIMER_BLINK, BLINK_TOGGLE_MS);
                blink_on = !blink_on;
                setLights((AMBER1_Pin | AMBER2_Pin) * blink_on);
            }
            if (isButtonPressed(BUTTON_INC)) edit_value = edit_value % 99 + 1;
            if (!isButtonPressed(BUTTON_SET)) break;
            // Save: green = red - amber must stay >= 1, otherwise refused
            if (edit_value < time_red) {
                time_amber = edit_value;
                time_green = time_red - time_amber;
            }
            edit_value = time_amber;
            break;

        case SET_GREEN:
            updateLedBuffer(4, edit_value);
            if (isButtonPressed(BUTTON_MODE)) {
                status = INIT;                  // Back to mode 1
                break;
            }
            if (isTimerExpired(TIMER_BLINK)) {
                setTimer(TIMER_BLINK, BLINK_TOGGLE_MS);
                blink_on = !blink_on;
                setLights((GREEN1_Pin | GREEN2_Pin) * blink_on);
            }
            if (isButtonPressed(BUTTON_INC)) edit_value = edit_value % 99 + 1;
            if (!isButtonPressed(BUTTON_SET)) break;
            // Save: red = amber + green must stay <= 99, otherwise refused
            if (time_amber + edit_value <= 99) {
                time_green = edit_value;
                time_red = time_amber + time_green;
            }
            edit_value = time_green;
            break;

        default:
            break;
    }
}
