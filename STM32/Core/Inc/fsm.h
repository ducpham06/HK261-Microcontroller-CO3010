#ifndef INC_FSM_H_
#define INC_FSM_H_

void fsm_automatic_enter(void);
void fsm_automatic(void);

void fsm_setting_enter(int newMode);    // MODE_RED, MODE_AMBER or MODE_GREEN
void fsm_setting(void);

void fsm_manual_enter(void);
void fsm_manual(void);

#endif /* INC_FSM_H_ */
