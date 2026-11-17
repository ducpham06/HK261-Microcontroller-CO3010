#include "fsm_automatic.h"
#include "global.h"
#include "button.h"
#include "software_timer.h"
#include "traffic_light.h"

void fsm_automatic(void) {
    switch (status) {
        case INIT:
            status = AUTO_R_G;
            countdown1 = time_red;
            countdown2 = time_green;
            setLights(RED1_Pin | GREEN2_Pin);
            setTimer(TIMER_COUNTDOWN, 1000);
            break;

        case AUTO_R_G:
            updateLedBuffer(countdown1, countdown2);
            if (isButtonPressed(BUTTON_MODE)) {
                status = SET_RED;
                edit_value = time_red;
                blink_on = 1;
                setLights(RED1_Pin | RED2_Pin);
                setTimer(TIMER_BLINK, BLINK_TOGGLE_MS);
                break;
            }
            if (isButtonPressed(BUTTON_INC)) {
                status = MAN_R_G;
                setLights(RED1_Pin | GREEN2_Pin);
                setTimer(TIMER_MANUAL, MANUAL_TIMEOUT_MS);
                break;
            }
            if (!isTimerExpired(TIMER_COUNTDOWN)) break;
            setTimer(TIMER_COUNTDOWN, 1000);
            countdown1--;
            countdown2--;
            if (countdown2 > 0) break;
            status = AUTO_R_A;
            countdown2 = time_amber;            // Road 1 stays red and keeps counting
            setLights(RED1_Pin | AMBER2_Pin);
            break;

        case AUTO_R_A:
            updateLedBuffer(countdown1, countdown2);
            if (isButtonPressed(BUTTON_MODE)) {
                status = SET_RED;
                edit_value = time_red;
                blink_on = 1;
                setLights(RED1_Pin | RED2_Pin);
                setTimer(TIMER_BLINK, BLINK_TOGGLE_MS);
                break;
            }
            if (isButtonPressed(BUTTON_INC)) {
                status = MAN_R_G;
                setLights(RED1_Pin | GREEN2_Pin);
                setTimer(TIMER_MANUAL, MANUAL_TIMEOUT_MS);
                break;
            }
            if (!isTimerExpired(TIMER_COUNTDOWN)) break;
            setTimer(TIMER_COUNTDOWN, 1000);
            countdown1--;
            countdown2--;
            if (countdown2 > 0) break;
            status = AUTO_G_R;
            countdown1 = time_green;
            countdown2 = time_red;
            setLights(GREEN1_Pin | RED2_Pin);
            break;

        case AUTO_G_R:
            updateLedBuffer(countdown1, countdown2);
            if (isButtonPressed(BUTTON_MODE)) {
                status = SET_RED;
                edit_value = time_red;
                blink_on = 1;
                setLights(RED1_Pin | RED2_Pin);
                setTimer(TIMER_BLINK, BLINK_TOGGLE_MS);
                break;
            }
            if (isButtonPressed(BUTTON_INC)) {
                status = MAN_R_G;
                setLights(RED1_Pin | GREEN2_Pin);
                setTimer(TIMER_MANUAL, MANUAL_TIMEOUT_MS);
                break;
            }
            if (!isTimerExpired(TIMER_COUNTDOWN)) break;
            setTimer(TIMER_COUNTDOWN, 1000);
            countdown1--;
            countdown2--;
            if (countdown1 > 0) break;
            status = AUTO_A_R;
            countdown1 = time_amber;            // Road 2 stays red and keeps counting
            setLights(AMBER1_Pin | RED2_Pin);
            break;

        case AUTO_A_R:
            updateLedBuffer(countdown1, countdown2);
            if (isButtonPressed(BUTTON_MODE)) {
                status = SET_RED;
                edit_value = time_red;
                blink_on = 1;
                setLights(RED1_Pin | RED2_Pin);
                setTimer(TIMER_BLINK, BLINK_TOGGLE_MS);
                break;
            }
            if (isButtonPressed(BUTTON_INC)) {
                status = MAN_R_G;
                setLights(RED1_Pin | GREEN2_Pin);
                setTimer(TIMER_MANUAL, MANUAL_TIMEOUT_MS);
                break;
            }
            if (!isTimerExpired(TIMER_COUNTDOWN)) break;
            setTimer(TIMER_COUNTDOWN, 1000);
            countdown1--;
            countdown2--;
            if (countdown1 > 0) break;
            status = AUTO_R_G;
            countdown1 = time_red;
            countdown2 = time_green;
            setLights(RED1_Pin | GREEN2_Pin);
            break;

        default:
            break;
    }
}
