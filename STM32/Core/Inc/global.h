/*
 * global.h
 *
 * Modes, traffic phases and shared state of the traffic light application
 */

#ifndef INC_GLOBAL_H_
#define INC_GLOBAL_H_

// Top-level modes. Button 1 cycles 1 -> 2 -> 3 -> 4 -> 1 (Lab 3 handout).
// MODE_MANUAL is an extra mode entered with Button 2 from normal mode.
typedef enum {
    MODE_NORMAL = 1,        // Mode 1: traffic light runs automatically
    MODE_MODIFY_RED,        // Mode 2: set the red duration
    MODE_MODIFY_AMBER,      // Mode 3: set the amber duration
    MODE_MODIFY_GREEN,      // Mode 4: set the green duration
    MODE_MANUAL             // Extra: step through the phases by hand
} app_mode_t;

// Traffic phases (road 1 color _ road 2 color), used by normal and manual mode
typedef enum {
    R_G = 0,                // Red 1,   Green 2
    R_A,                    // Red 1,   Amber 2
    G_R,                    // Green 1, Red 2
    A_R,                    // Amber 1, Red 2
    NUM_OF_PHASES
} phase_t;

#define MIN_DURATION        1       // Allowed duration range in seconds
#define MAX_DURATION        99
#define DEFAULT_RED         5
#define DEFAULT_AMBER       2
#define DEFAULT_GREEN       3
#define MANUAL_TIMEOUT_MS   10000   // Manual mode returns to normal after 10 s idle
#define BLINK_TOGGLE_MS     250     // Toggle every 250 ms -> 2 Hz blinking

extern app_mode_t mode;
extern phase_t phase;

// Durations in seconds. Always kept consistent: time_red = time_amber + time_green
extern int time_red;
extern int time_amber;
extern int time_green;

// Seconds left for each road in normal mode
extern int countdown_road1;
extern int countdown_road2;

#endif /* INC_GLOBAL_H_ */
