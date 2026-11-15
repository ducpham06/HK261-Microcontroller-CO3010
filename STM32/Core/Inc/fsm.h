/*
 * fsm.h
 *
 * Traffic light FSMs, each run as a scheduler task every 10 ms.
 * Only the task that owns the current mode does any work; the others return
 * immediately. Each task handles Button 1 (mode select) for its own modes.
 *
 *  - fsm_automatic : Mode 1, R_G -> R_A -> G_R -> A_R on timer events
 *  - fsm_setting   : Modes 2-4, edit and save the red/amber/green durations
 *  - fsm_manual    : extra manual mode, phases stepped by Button 2
 *
 * Mode changes (Button 1):
 *   NORMAL -> MODIFY_RED -> MODIFY_AMBER -> MODIFY_GREEN -> NORMAL
 *   MANUAL -> NORMAL
 */

#ifndef INC_FSM_H_
#define INC_FSM_H_

#include "global.h"

// Called once before the scheduler starts
void fsm_init(void);

// Leave the current mode and run the entry actions of the new one
void fsm_enter_mode(app_mode_t newMode);

// Shared output actions
void setPhaseLights(phase_t p);
void fsm_update_display(void);

// Scheduler tasks
void fsm_automatic(void);
void fsm_setting(void);
void fsm_manual(void);

// Entry actions of each mode
void fsm_automatic_enter(void);
void fsm_setting_enter(void);
void fsm_manual_enter(void);

// Value being edited in modes 2-4 (shown on the display)
int fsm_setting_getValue(void);

#endif /* INC_FSM_H_ */
