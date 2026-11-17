#ifndef INC_GLOBAL_H_
#define INC_GLOBAL_H_

// States. Road 1 color _ road 2 color.

// Mode 1 (fsm_automatic)
#define AUTO_R_G            1
#define AUTO_R_A            2
#define AUTO_G_R            3
#define AUTO_A_R            4

// Modes 2-4 (fsm_setting)
#define SET_RED             12
#define SET_AMBER           13
#define SET_GREEN           14

// Manual mode (fsm_manual)
#define MAN_R_G             21
#define MAN_R_A             22
#define MAN_G_R             23
#define MAN_A_R             24

#define MANUAL_TIMEOUT_MS   10000
#define BLINK_TOGGLE_MS     250     // Toggle every 250 ms -> 2 Hz

extern int status;

// Durations in seconds, always time_red = time_amber + time_green
extern int time_red;
extern int time_amber;
extern int time_green;

// Seconds left on each road in mode 1
extern int countdown1;
extern int countdown2;

// Modes 2-4: value being edited, and whether the LEDs are lit in the blink
extern int edit_value;
extern int blink_on;

#endif /* INC_GLOBAL_H_ */
