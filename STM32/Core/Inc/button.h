#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

#define BUTTON_MODE     0   // Button 1: select mode
#define BUTTON_INC      1   // Button 2: increase value
#define BUTTON_SET      2   // Button 3: save value
#define NO_OF_BUTTONS   3

// Called every TIMER_CYCLE (10 ms)
void getKeyInput(void);

// 1 once per press, then cleared. A press nobody reads is dropped
// at the next getKeyInput(), so it can never act in a later mode.
int isButtonPressed(int index);

#endif /* INC_BUTTON_H_ */
