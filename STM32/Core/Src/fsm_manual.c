#include "fsm_manual.h"
#include "global.h"
#include "button.h"
#include "software_timer.h"
#include "traffic_light.h"

void fsm_manual(void) {
    switch (status) {
        case MAN_R_G:
            updateLedBuffer(0, 1);
            if (isButtonPressed(BUTTON_MODE) || isTimerExpired(TIMER_MANUAL)) {
                status = INIT;                  // Back to mode 1
                break;
            }
            if (!isButtonPressed(BUTTON_INC)) break;
            status = MAN_R_A;
            setLights(RED1_Pin | AMBER2_Pin);
            setTimer(TIMER_MANUAL, MANUAL_TIMEOUT_MS);
            break;

        case MAN_R_A:
            updateLedBuffer(0, 2);
            if (isButtonPressed(BUTTON_MODE) || isTimerExpired(TIMER_MANUAL)) {
                status = INIT;
                break;
            }
            if (!isButtonPressed(BUTTON_INC)) break;
            status = MAN_G_R;
            setLights(GREEN1_Pin | RED2_Pin);
            setTimer(TIMER_MANUAL, MANUAL_TIMEOUT_MS);
            break;

        case MAN_G_R:
            updateLedBuffer(0, 3);
            if (isButtonPressed(BUTTON_MODE) || isTimerExpired(TIMER_MANUAL)) {
                status = INIT;
                break;
            }
            if (!isButtonPressed(BUTTON_INC)) break;
            status = MAN_A_R;
            setLights(AMBER1_Pin | RED2_Pin);
            setTimer(TIMER_MANUAL, MANUAL_TIMEOUT_MS);
            break;

        case MAN_A_R:
            updateLedBuffer(0, 4);
            if (isButtonPressed(BUTTON_MODE) || isTimerExpired(TIMER_MANUAL)) {
                status = INIT;
                break;
            }
            if (!isButtonPressed(BUTTON_INC)) break;
            status = MAN_R_G;
            setLights(RED1_Pin | GREEN2_Pin);
            setTimer(TIMER_MANUAL, MANUAL_TIMEOUT_MS);
            break;

        default:
            break;
    }
}
