/*
 * scheduler.c
 *
 * Cooperative scheduler implementation
 */

#include "scheduler.h"
#include "main.h"

typedef struct sTask {
    void (*pTask)(void);
    uint32_t delay;         // Ticks after the previous task in the list
    uint32_t period;        // Ticks between runs, 0 = one-shot
    uint32_t taskID;
    uint8_t used;
    struct sTask *next;
} sTask;

// Fixed task table: no malloc, so adding a task can never fail on heap space
static sTask taskPool[SCH_MAX_TASKS];
static sTask *head = 0;

// Written by the interrupt, read by the main loop
static volatile uint32_t pendingTicks = 0;

static uint32_t nextTaskID = 1;

// Task currently being executed (it is outside the list while it runs)
static sTask *runningTask = 0;
static uint8_t runningTaskDeleted = 0;

static uint32_t msToTicks(uint32_t ms) {
    return (ms + SCH_TICK_MS - 1) / SCH_TICK_MS;   // Round up: never run early
}

// Insert a task that is due in delayTicks. Tasks with the same due time
// keep their order (the new task goes after them).
static void insertTask(sTask *task, uint32_t delayTicks) {
    sTask *prev = 0;
    sTask *cur = head;

    while (cur != 0 && delayTicks >= cur->delay) {
        delayTicks -= cur->delay;
        prev = cur;
        cur = cur->next;
    }

    task->delay = delayTicks;
    task->next = cur;
    if (cur != 0) {
        cur->delay -= delayTicks;
    }
    if (prev == 0) {
        head = task;
    } else {
        prev->next = task;
    }
}

static void freeTask(sTask *task) {
    task->used = 0;
    task->pTask = 0;
    task->next = 0;
}

void SCH_Init(void) {
    for (int i = 0; i < SCH_MAX_TASKS; i++) {
        freeTask(&taskPool[i]);
    }
    head = 0;
    runningTask = 0;
    runningTaskDeleted = 0;
    pendingTicks = 0;
}

void SCH_Update(void) {
    pendingTicks++;
}

uint32_t SCH_Add_Task(void (*pFunction)(void), uint32_t delay_ms, uint32_t period_ms) {
    if (pFunction == 0) return SCH_NO_TASK_ID;

    sTask *task = 0;
    for (int i = 0; i < SCH_MAX_TASKS; i++) {
        if (!taskPool[i].used) {
            task = &taskPool[i];
            break;
        }
    }
    if (task == 0) return SCH_NO_TASK_ID;     // Table full

    task->used = 1;
    task->pTask = pFunction;
    task->period = msToTicks(period_ms);
    if (period_ms > 0 && task->period == 0) {
        task->period = 1;
    }
    task->taskID = nextTaskID++;
    if (nextTaskID == SCH_NO_TASK_ID) {
        nextTaskID = 1;                       // Skip the reserved ID on wrap-around
    }

    insertTask(task, msToTicks(delay_ms));
    return task->taskID;
}

uint8_t SCH_Delete_Task(uint32_t taskID) {
    if (taskID == SCH_NO_TASK_ID) return 0;

    // A task deleting itself while it runs: just do not reschedule it
    if (runningTask != 0 && runningTask->taskID == taskID) {
        runningTaskDeleted = 1;
        return 1;
    }

    sTask *prev = 0;
    sTask *cur = head;
    while (cur != 0) {
        if (cur->taskID == taskID) {
            if (cur->next != 0) {
                cur->next->delay += cur->delay;   // Keep later tasks' due times
            }
            if (prev == 0) {
                head = cur->next;
            } else {
                prev->next = cur->next;
            }
            freeTask(cur);
            return 1;
        }
        prev = cur;
        cur = cur->next;
    }
    return 0;
}

void SCH_Dispatch_Tasks(void) {
    for (;;) {
        // Run every task that is due now, in list order
        if (head != 0 && head->delay == 0) {
            sTask *task = head;
            head = task->next;
            task->next = 0;

            runningTask = task;
            runningTaskDeleted = 0;
            task->pTask();
            runningTask = 0;

            if (task->period > 0 && !runningTaskDeleted) {
                insertTask(task, task->period);
            } else {
                freeTask(task);
            }
            continue;
        }

        // Nothing due: apply one tick counted by the interrupt, if any
        __disable_irq();
        if (pendingTicks == 0) {
            __enable_irq();
            return;
        }
        pendingTicks--;
        __enable_irq();

        if (head != 0 && head->delay > 0) {
            head->delay--;
        }
    }
}
