/*
 * fsm_setting.c
 *
 * Modes 2, 3, 4: set the red, amber or green duration.
 *   - The LEDs of that color blink at 2 Hz on both roads.
 *   - Button 2 increases the value (1-99, then back to 1).
 *   - Button 3 saves it. Button 1 goes to the next mode and drops an unsaved value.
 *   - Display: mode number (02/03/04) on the left, value on the right.
 *
 * The cycle needs time_red = time_amber + time_green, so a save also changes
 * one other value: saving red or amber sets green = red - amber, saving green
 * sets red = amber + green. A save that would put any value outside 1-99 is
 * refused and the stored value is shown again.
 */

#include "fsm.h"
#include "global.h"
#include "button.h"
#include "software_timer.h"
#include "traffic_light.h"

static int value;       // Value being edited
static int blinkOn;

static int storedValue(void) {
    switch (mode) {
        case MODE_RED:   return time_red;
        case MODE_AMBER: return time_amber;
        default:         return time_green;
    }
}

// Both LEDs of the color being set
static uint16_t colorPins(void) {
    switch (mode) {
        case MODE_RED:   return RED1_Pin | RED2_Pin;
        case MODE_AMBER: return AMBER1_Pin | AMBER2_Pin;
        default:         return GREEN1_Pin | GREEN2_Pin;
    }
}

static void showBlink(void) {
    uint16_t pins = 0;
    if (blinkOn) pins = colorPins();
    setLights(pins);
}

// Returns 0 if the value is refused
static int saveValue(void) {
    int red = time_red;
    int amber = time_amber;
    int green = time_green;

    switch (mode) {
        case MODE_RED:   red = value;   green = red - amber; break;
        case MODE_AMBER: amber = value; green = red - amber; break;
        case MODE_GREEN: green = value; red = amber + green; break;
    }

    if (red < 1 || red > 99 || amber < 1 || green < 1) return 0;

    time_red = red;
    time_amber = amber;
    time_green = green;
    return 1;
}

static void goToNextMode(void) {
    if (mode == MODE_GREEN) {
        fsm_automatic_enter();
        return;
    }
    fsm_setting_enter(mode + 1);
}

void fsm_setting_enter(int newMode) {
    mode = newMode;
    value = storedValue();
    blinkOn = 1;
    showBlink();
    setTimer(TIMER_BLINK, BLINK_TOGGLE_MS);
    updateLedBuffer(mode, value);
}

void fsm_setting(void) {
    if (mode != MODE_RED && mode != MODE_AMBER && mode != MODE_GREEN) return;

    if (isButtonPressed(BUTTON_MODE)) {
        goToNextMode();
        return;
    }

    if (isButtonPressed(BUTTON_INC)) value = value % 99 + 1;
    if (isButtonPressed(BUTTON_SET) && !saveValue()) value = storedValue();

    if (isTimerExpired(TIMER_BLINK)) {
        setTimer(TIMER_BLINK, BLINK_TOGGLE_MS);
        blinkOn = !blinkOn;
        showBlink();
    }

    updateLedBuffer(mode, value);
}
