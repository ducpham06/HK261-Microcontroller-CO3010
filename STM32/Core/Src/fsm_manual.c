/*
 * fsm_manual.c
 *
 * Extra manual mode (entered with Button 2 from normal mode):
 *   Button 2 steps R_G -> R_A -> G_R -> A_R -> R_G.
 *   Button 1 or 10 s without a press returns to normal mode.
 */

#include "fsm.h"
#include "button.h"
#include "software_timer.h"

void fsm_manual_enter(void) {
    phase = R_G;
    setPhaseLights(phase);
    setTimer(TIMER_MANUAL_TIMEOUT, MANUAL_TIMEOUT_MS);
}

void fsm_manual_run(void) {
    if (isButtonPressed(BUTTON_INC)) {
        switch (phase) {
            case R_G: phase = R_A; break;
            case R_A: phase = G_R; break;
            case G_R: phase = A_R; break;
            case A_R: phase = R_G; break;
            default:  phase = R_G; break;
        }
        setPhaseLights(phase);
        setTimer(TIMER_MANUAL_TIMEOUT, MANUAL_TIMEOUT_MS);   // Restart the idle timeout
    }

    if (isTimerExpired(TIMER_MANUAL_TIMEOUT)) {
        fsm_enter_mode(MODE_NORMAL);
    }
}
