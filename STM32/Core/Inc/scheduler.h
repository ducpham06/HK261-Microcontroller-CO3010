/*
 * scheduler.h
 *
 * Cooperative scheduler (Lab 4).
 *
 *  - SCH_Update() is called from the TIM2 interrupt every SCH_TICK_MS.
 *    It only counts ticks, so it is O(1) and never touches the task list.
 *  - SCH_Dispatch_Tasks() is called from the main loop. It applies the
 *    counted ticks to the task list and runs every task that is due.
 *  - Tasks are kept in a list sorted by due time. Each task stores its delay
 *    relative to the task before it, so only the first task is decremented.
 *  - Tasks due on the same tick run in the order they were added.
 *
 * Delays and periods are given in milliseconds.
 */

#ifndef INC_SCHEDULER_H_
#define INC_SCHEDULER_H_

#include <stdint.h>

#define SCH_TICK_MS         10      // Must match the TIM2 interrupt period
#define SCH_MAX_TASKS       16
#define SCH_NO_TASK_ID      0

void SCH_Init(void);

// Called from the timer interrupt
void SCH_Update(void);

// Called in the main loop
void SCH_Dispatch_Tasks(void);

// Run pFunction after delay_ms, then every period_ms (period_ms = 0: run once).
// Returns the task ID, or SCH_NO_TASK_ID if the task table is full.
uint32_t SCH_Add_Task(void (*pFunction)(void), uint32_t delay_ms, uint32_t period_ms);

// Remove a task. Returns 1 on success, 0 if the ID does not exist.
uint8_t SCH_Delete_Task(uint32_t taskID);

#endif /* INC_SCHEDULER_H_ */
