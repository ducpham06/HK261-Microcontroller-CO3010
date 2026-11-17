#include "fsm.h"
#include "global.h"
#include "button.h"
#include "software_timer.h"
#include "traffic_light.h"

static void enterPhase(int newPhase) {
    phase = newPhase;
    setPhaseLights(phase);

    switch (phase) {
        case R_G:
            countdown1 = time_red;
            countdown2 = time_green;
            break;
        case R_A:
            countdown2 = time_amber;    // Road 1 stays red and keeps counting
            break;
        case G_R:
            countdown1 = time_green;
            countdown2 = time_red;
            break;
        case A_R:
            countdown1 = time_amber;    // Road 2 stays red and keeps counting
            break;
    }

    setTimer(TIMER_COUNTDOWN, 1000);
    updateLedBuffer(countdown1, countdown2);
}

void fsm_automatic_enter(void) {
    mode = MODE_NORMAL;
    enterPhase(R_G);
}

void fsm_automatic(void) {
    if (mode != MODE_NORMAL) return;

    if (isButtonPressed(BUTTON_MODE)) {
        fsm_setting_enter(MODE_RED);
        return;
    }
    if (isButtonPressed(BUTTON_INC)) {
        fsm_manual_enter();
        return;
    }
    if (!isTimerExpired(TIMER_COUNTDOWN)) return;

    // One second has passed
    setTimer(TIMER_COUNTDOWN, 1000);
    countdown1--;
    countdown2--;
    updateLedBuffer(countdown1, countdown2);

    switch (phase) {
        case R_G:
            if (countdown2 == 0) enterPhase(R_A);
            break;
        case R_A:
            if (countdown2 == 0) enterPhase(G_R);
            break;
        case G_R:
            if (countdown1 == 0) enterPhase(A_R);
            break;
        case A_R:
            if (countdown1 == 0) enterPhase(R_G);
            break;
    }
}
