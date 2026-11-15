/*
 * fsm.c
 *
 * Shared parts of the traffic light FSMs: mode changes, phase lights and
 * the 7-segment output for each mode.
 */

#include "fsm.h"
#include "button.h"
#include "software_timer.h"
#include "traffic_light.h"
#include "display7seg.h"

// Light colors of each phase: phaseColors[phase][road]
static const light_color_t phaseColors[NUM_OF_PHASES][NUM_OF_ROADS] = {
    [R_G] = {COLOR_RED,   COLOR_GREEN},
    [R_A] = {COLOR_RED,   COLOR_AMBER},
    [G_R] = {COLOR_GREEN, COLOR_RED},
    [A_R] = {COLOR_AMBER, COLOR_RED},
};

void setPhaseLights(phase_t p) {
    if (p >= NUM_OF_PHASES) return;
    setRoadColor(ROAD_1, phaseColors[p][ROAD_1]);
    setRoadColor(ROAD_2, phaseColors[p][ROAD_2]);
}

void fsm_enter_mode(app_mode_t newMode) {
    mode = newMode;

    // Stop the timers owned by the previous mode
    clearTimer(TIMER_COUNTDOWN);
    clearTimer(TIMER_MANUAL_TIMEOUT);
    clearTimer(TIMER_BLINK);

    switch (mode) {
        case MODE_MODIFY_RED:
        case MODE_MODIFY_AMBER:
        case MODE_MODIFY_GREEN:
            fsm_setting_enter();
            break;
        case MODE_MANUAL:
            fsm_manual_enter();
            break;
        case MODE_NORMAL:
        default:
            mode = MODE_NORMAL;
            fsm_automatic_enter();
            break;
    }
}

// What the four 7-segment digits show in each mode
void fsm_update_display(void) {
    switch (mode) {
        case MODE_NORMAL:
            // Left: road 1 countdown, right: road 2 countdown
            display_setNumbers(countdown_road1, countdown_road2);
            break;
        case MODE_MODIFY_RED:
        case MODE_MODIFY_AMBER:
        case MODE_MODIFY_GREEN:
            // Left: mode number 02/03/04, right: value being edited
            display_setNumbers((int)mode, fsm_setting_getValue());
            break;
        case MODE_MANUAL:
            // Left: 00, right: phase number 01-04
            display_setNumbers(0, (int)phase + 1);
            break;
        default:
            break;
    }
}

void fsm_init(void) {
    time_red = DEFAULT_RED;
    time_amber = DEFAULT_AMBER;
    time_green = DEFAULT_GREEN;

    button_init();
    display_init();
    clearAllTrafficLights();
    fsm_enter_mode(MODE_NORMAL);
    fsm_update_display();
}
