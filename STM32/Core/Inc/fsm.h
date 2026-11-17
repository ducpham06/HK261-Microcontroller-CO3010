/*
 * fsm.h
 *
 * Each FSM returns at once unless it owns the current mode, so all three
 * can be called every 10 ms. A mode is entered through its enter function.
 *
 *   mode 1 --B1--> mode 2 --B1--> mode 3 --B1--> mode 4 --B1--> mode 1
 *   mode 1 --B2--> manual --B1 or 10 s idle--> mode 1
 */

#ifndef INC_FSM_H_
#define INC_FSM_H_

void fsm_automatic_enter(void);
void fsm_automatic(void);

void fsm_setting_enter(int newMode);    // MODE_RED, MODE_AMBER or MODE_GREEN
void fsm_setting(void);

void fsm_manual_enter(void);
void fsm_manual(void);

#endif /* INC_FSM_H_ */
