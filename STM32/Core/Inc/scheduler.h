#ifndef INC_SCHEDULER_H_
#define INC_SCHEDULER_H_

#include <stdint.h>

#define SCH_TICK_MS     10      // Must equal the TIM2 period
#define SCH_MAX_TASKS   16

void SCH_Update(void);
void SCH_Dispatch_Tasks(void);

// Returns the task ID, or 0 if all SCH_MAX_TASKS slots are used
uint32_t SCH_Add_Task(void (*pFunction)(void), uint32_t delay_ms, uint32_t period_ms);

// Returns 1 if the task was removed, 0 if the ID was not found
uint8_t SCH_Delete_Task(uint32_t taskID);

#endif /* INC_SCHEDULER_H_ */
