#include "fsm_manual.h"
#include "fsm_automatic.h"
#include "global.h"
#include "button.h"
#include "software_timer.h"
#include "traffic_light.h"

void fsm_manual_enter(void) {
    mode = MODE_MANUAL;
    phase = R_G;
    setPhaseLights(phase);
    setTimer(TIMER_MANUAL, MANUAL_TIMEOUT_MS);
    updateLedBuffer(0, phase + 1);
}

void fsm_manual(void) {
    if (mode != MODE_MANUAL) return;

    if (isButtonPressed(BUTTON_MODE) || isTimerExpired(TIMER_MANUAL)) {
        fsm_automatic_enter();
        return;
    }
    if (!isButtonPressed(BUTTON_INC)) return;

    phase = (phase + 1) % 4;        // Phases are numbered in cycle order
    setPhaseLights(phase);
    setTimer(TIMER_MANUAL, MANUAL_TIMEOUT_MS);
    updateLedBuffer(0, phase + 1);
}
