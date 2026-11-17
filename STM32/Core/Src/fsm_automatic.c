#include "fsm_automatic.h"
#include "global.h"
#include "button.h"
#include "software_timer.h"
#include "traffic_light.h"

void fsm_automatic(void) {
    if (mode != MODE_NORMAL) return;

    updateLedBuffer(countdown1, countdown2);

    // Button 1: go to mode 2 (set red)
    if (isButtonPressed(BUTTON_MODE)) {
        mode = MODE_RED;
        edit_value = time_red;
        blink_on = 1;
        setLights(RED1_Pin | RED2_Pin);
        setTimer(TIMER_BLINK, BLINK_TOGGLE_MS);
        return;
    }

    // Button 2: go to manual mode
    if (isButtonPressed(BUTTON_INC)) {
        mode = MODE_MANUAL;
        phase = R_G;
        setLights(RED1_Pin | GREEN2_Pin);
        setTimer(TIMER_MANUAL, MANUAL_TIMEOUT_MS);
        return;
    }

    if (!isTimerExpired(TIMER_COUNTDOWN)) return;

    // One second has passed
    setTimer(TIMER_COUNTDOWN, 1000);
    countdown1--;
    countdown2--;

    switch (phase) {
        case R_G:
            if (countdown2 > 0) break;
            phase = R_A;
            countdown2 = time_amber;        // Road 1 stays red and keeps counting
            setLights(RED1_Pin | AMBER2_Pin);
            break;
        case R_A:
            if (countdown2 > 0) break;
            phase = G_R;
            countdown1 = time_green;
            countdown2 = time_red;
            setLights(GREEN1_Pin | RED2_Pin);
            break;
        case G_R:
            if (countdown1 > 0) break;
            phase = A_R;
            countdown1 = time_amber;        // Road 2 stays red and keeps counting
            setLights(AMBER1_Pin | RED2_Pin);
            break;
        case A_R:
            if (countdown1 > 0) break;
            phase = R_G;
            countdown1 = time_red;
            countdown2 = time_green;
            setLights(RED1_Pin | GREEN2_Pin);
            break;
    }
}
