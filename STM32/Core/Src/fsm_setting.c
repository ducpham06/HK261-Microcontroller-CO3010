/*
 * fsm_setting.c
 *
 * Modes 2, 3, 4: modify the red, amber or green duration.
 *   - The LEDs of the selected color blink at 2 Hz on both roads.
 *   - Button 2 increases the value being edited (1-99, wraps to 1).
 *   - Button 3 saves it. Unsaved changes are dropped when Button 1 changes mode.
 *
 * The cycle needs time_red = time_amber + time_green, so a save also updates
 * one other duration:
 *   - saving red or amber recomputes green = red - amber
 *   - saving green recomputes red = amber + green
 * A save that would push any duration outside 1-99 is refused, and the
 * display returns to the stored value.
 */

#include "fsm.h"
#include "button.h"
#include "software_timer.h"
#include "traffic_light.h"

static int editValue;   // Value shown and changed by Button 2
static int blinkOn;     // Current blink state of the selected LEDs

static light_color_t selectedColor(void) {
    switch (mode) {
        case MODE_MODIFY_RED:   return COLOR_RED;
        case MODE_MODIFY_AMBER: return COLOR_AMBER;
        default:                return COLOR_GREEN;
    }
}

static int storedValue(void) {
    switch (mode) {
        case MODE_MODIFY_RED:   return time_red;
        case MODE_MODIFY_AMBER: return time_amber;
        default:                return time_green;
    }
}

static int inRange(int value) {
    return value >= MIN_DURATION && value <= MAX_DURATION;
}

// Returns 1 if the value was saved, 0 if it was refused
static int saveValue(int value) {
    int red = time_red;
    int amber = time_amber;
    int green = time_green;

    switch (mode) {
        case MODE_MODIFY_RED:
            red = value;
            green = red - amber;
            break;
        case MODE_MODIFY_AMBER:
            amber = value;
            green = red - amber;
            break;
        case MODE_MODIFY_GREEN:
            green = value;
            red = amber + green;
            break;
        default:
            return 0;
    }

    if (!inRange(red) || !inRange(amber) || !inRange(green)) {
        return 0;
    }

    time_red = red;
    time_amber = amber;
    time_green = green;
    return 1;
}

static void showBlink(void) {
    setBothRoads(blinkOn ? selectedColor() : COLOR_OFF);
}

void fsm_setting_enter(void) {
    editValue = storedValue();
    blinkOn = 1;
    showBlink();
    setTimer(TIMER_BLINK, BLINK_TOGGLE_MS);
}

void fsm_setting_run(void) {
    if (isButtonPressed(BUTTON_INC)) {
        editValue = (editValue >= MAX_DURATION) ? MIN_DURATION : editValue + 1;
    }

    if (isButtonPressed(BUTTON_SET)) {
        if (!saveValue(editValue)) {
            editValue = storedValue();      // Refused: show the stored value again
        }
    }

    if (isTimerExpired(TIMER_BLINK)) {
        setTimer(TIMER_BLINK, BLINK_TOGGLE_MS);
        blinkOn = !blinkOn;
        showBlink();
    }
}

int fsm_setting_getValue(void) {
    return editValue;
}
