#ifndef INC_GLOBAL_H_
#define INC_GLOBAL_H_

// Modes. Button 1 cycles 1 -> 2 -> 3 -> 4 -> 1.
// Manual mode is an extra, entered with Button 2 from mode 1.
#define MODE_NORMAL         1
#define MODE_RED            2
#define MODE_AMBER          3
#define MODE_GREEN          4
#define MODE_MANUAL         5

// Phases (road 1 color, road 2 color), in cycle order
#define R_G                 0
#define R_A                 1
#define G_R                 2
#define A_R                 3

#define MANUAL_TIMEOUT_MS   10000
#define BLINK_TOGGLE_MS     250     // Toggle every 250 ms -> 2 Hz

extern int mode;
extern int phase;

// Durations in seconds, always time_red = time_amber + time_green
extern int time_red;
extern int time_amber;
extern int time_green;

// Seconds left on each road in mode 1
extern int countdown1;
extern int countdown2;

#endif /* INC_GLOBAL_H_ */
