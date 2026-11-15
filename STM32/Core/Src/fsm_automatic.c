/*
 * fsm_automatic.c
 *
 * Mode 1 (normal): R_G -> R_A -> G_R -> A_R -> R_G
 * A phase ends when the countdown of the road that is not red reaches 0.
 * Button 2 switches to the extra manual mode.
 */

#include "fsm.h"
#include "button.h"
#include "software_timer.h"

// Entry actions of a phase: lights, countdown values, 1 s tick
static void enterPhase(phase_t p) {
    phase = p;
    setPhaseLights(p);

    switch (p) {
        case R_G:
            countdown_road1 = time_red;
            countdown_road2 = time_green;
            break;
        case R_A:
            // Road 1 stays red and keeps counting (time_red = time_green + time_amber)
            countdown_road2 = time_amber;
            break;
        case G_R:
            countdown_road1 = time_green;
            countdown_road2 = time_red;
            break;
        case A_R:
            // Road 2 stays red and keeps counting
            countdown_road1 = time_amber;
            break;
        default:
            break;
    }

    setTimer(TIMER_COUNTDOWN, 1000);
}

void fsm_automatic_enter(void) {
    enterPhase(R_G);
}

void fsm_automatic_run(void) {
    if (isButtonPressed(BUTTON_INC)) {
        fsm_enter_mode(MODE_MANUAL);
        return;
    }

    if (!isTimerExpired(TIMER_COUNTDOWN)) {
        return;
    }

    // One second has passed
    setTimer(TIMER_COUNTDOWN, 1000);
    if (countdown_road1 > 0) countdown_road1--;
    if (countdown_road2 > 0) countdown_road2--;

    switch (phase) {
        case R_G:
            if (countdown_road2 == 0) enterPhase(R_A);
            break;
        case R_A:
            if (countdown_road2 == 0) enterPhase(G_R);
            break;
        case G_R:
            if (countdown_road1 == 0) enterPhase(A_R);
            break;
        case A_R:
            if (countdown_road1 == 0) enterPhase(R_G);
            break;
        default:
            enterPhase(R_G);
            break;
    }
}
