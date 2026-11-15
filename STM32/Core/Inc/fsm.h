/*
 * fsm.h
 *
 * Two-level FSM:
 *  - fsm.c           : mode FSM (Button 1 selects the mode) and display output
 *  - fsm_automatic.c : Mode 1, phases R_G -> R_A -> G_R -> A_R on timer events
 *  - fsm_manual.c    : extra manual mode, phases stepped by Button 2
 *  - fsm_setting.c   : Modes 2-4, edit and save the red/amber/green durations
 */

#ifndef INC_FSM_H_
#define INC_FSM_H_

#include "global.h"

// Called once before TIM2 starts
void fsm_init(void);

// Called on every pass of the main loop
void fsm_run(void);

// Leave the current mode and run the entry actions of the new one
void fsm_enter_mode(app_mode_t newMode);

// Shared output action: show the lights of a phase on both roads
void setPhaseLights(phase_t p);

void fsm_automatic_enter(void);
void fsm_automatic_run(void);

void fsm_manual_enter(void);
void fsm_manual_run(void);

void fsm_setting_enter(void);
void fsm_setting_run(void);
int  fsm_setting_getValue(void);    // Value being edited (shown on the display)

#endif /* INC_FSM_H_ */
