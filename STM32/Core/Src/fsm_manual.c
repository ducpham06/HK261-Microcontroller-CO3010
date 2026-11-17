#include "fsm_manual.h"
#include "global.h"
#include "button.h"
#include "software_timer.h"
#include "traffic_light.h"

void fsm_manual(void) {
    if (mode != MODE_MANUAL) return;

    updateLedBuffer(0, phase + 1);

    // Back to mode 1, starting again from R_G
    if (isButtonPressed(BUTTON_MODE) || isTimerExpired(TIMER_MANUAL)) {
        mode = MODE_NORMAL;
        phase = R_G;
        countdown1 = time_red;
        countdown2 = time_green;
        setLights(RED1_Pin | GREEN2_Pin);
        setTimer(TIMER_COUNTDOWN, 1000);
        return;
    }

    if (!isButtonPressed(BUTTON_INC)) return;

    setTimer(TIMER_MANUAL, MANUAL_TIMEOUT_MS);      // Restart the idle timeout
    switch (phase) {
        case R_G:
            phase = R_A;
            setLights(RED1_Pin | AMBER2_Pin);
            break;
        case R_A:
            phase = G_R;
            setLights(GREEN1_Pin | RED2_Pin);
            break;
        case G_R:
            phase = A_R;
            setLights(AMBER1_Pin | RED2_Pin);
            break;
        case A_R:
            phase = R_G;
            setLights(RED1_Pin | GREEN2_Pin);
            break;
    }
}
