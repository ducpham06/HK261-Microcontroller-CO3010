#include "main.h"
#include "scheduler.h"

typedef struct Task {
    void (*pTask)(void);
    uint32_t delay;         // Ticks after the task before it in the list
    uint32_t period;        // Ticks between runs, 0 = run once
    uint32_t id;            // 0 = slot is free
    struct Task *next;
} Task;

static Task tasks[SCH_MAX_TASKS];   // Fixed slots, no malloc
static Task *head = 0;              // First task to run
static Task *running = 0;           // Task being run (not in the list)
static uint32_t nextID = 1;
static volatile uint32_t pendingTicks = 0;

// Put a task into the sorted list, due in `delay` ticks.
// `link` walks the list's next-pointers, so inserting at the head
// needs no special case.
static void insertTask(Task *task, uint32_t delay) {
    Task **link = &head;
    while (*link != 0 && delay >= (*link)->delay) {
        delay -= (*link)->delay;
        link = &(*link)->next;
    }

    task->delay = delay;
    task->next = *link;
    if (task->next != 0) task->next->delay -= delay;
    *link = task;
}

static void runDueTasks(void) {
    while (head != 0 && head->delay == 0) {
        Task *task = head;
        head = task->next;

        running = task;
        task->pTask();
        running = 0;

        if (task->period == 0) {
            task->id = 0;           // One-shot (or deleted while running): free the slot
            continue;
        }
        insertTask(task, task->period);
    }
}

// Take one tick counted by the interrupt. Returns 0 if there is none.
static int takeTick(void) {
    __disable_irq();
    int hasTick = (pendingTicks > 0);
    pendingTicks -= hasTick;
    __enable_irq();
    return hasTick;
}

void SCH_Update(void) {
    pendingTicks++;
}

void SCH_Dispatch_Tasks(void) {
    runDueTasks();
    while (takeTick()) {
        if (head != 0) head->delay--;   // > 0 here: due tasks have already run
        runDueTasks();
    }
}

uint32_t SCH_Add_Task(void (*pFunction)(void), uint32_t delay_ms, uint32_t period_ms) {
    int i = 0;
    while (i < SCH_MAX_TASKS && tasks[i].id != 0) i++;
    if (i == SCH_MAX_TASKS) return 0;

    Task *task = &tasks[i];
    task->pTask = pFunction;
    task->period = (period_ms + SCH_TICK_MS - 1) / SCH_TICK_MS;
    task->id = nextID++;
    insertTask(task, (delay_ms + SCH_TICK_MS - 1) / SCH_TICK_MS);
    return task->id;
}

uint8_t SCH_Delete_Task(uint32_t taskID) {
    if (taskID == 0) return 0;

    // A task deleting itself: make it one-shot so it is freed when it returns
    if (running != 0 && running->id == taskID) {
        running->period = 0;
        return 1;
    }

    Task **link = &head;
    while (*link != 0 && (*link)->id != taskID) link = &(*link)->next;
    if (*link == 0) return 0;

    Task *task = *link;
    *link = task->next;
    if (task->next != 0) task->next->delay += task->delay;   // Keep later due times
    task->id = 0;
    return 1;
}
