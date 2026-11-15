# Lab 4 – Cooperative scheduler on top of the Lab 3 traffic light

## How it runs

```
TIM2 interrupt, every 10 ms  ->  SCH_Update()          (counts one tick, O(1))
main loop                    ->  SCH_Dispatch_Tasks()  (runs every task that is due)
```

| Task | Period | Job |
|---|---|---|
| `Led_Blink` | 1000 ms | Status LED (CubeMX pin labelled `LED`) |
| `timer_Run` | 10 ms | Lab 3 software timers (1 s countdown, 2 Hz blink, 10 s manual timeout) |
| `getKeyInput` | 10 ms | 3-sample debounce, Button 2 auto-repeat |
| `fsm_manual` | 10 ms | Extra manual mode |
| `fsm_automatic` | 10 ms | Mode 1 |
| `fsm_setting` | 10 ms | Modes 2–4 |
| `display_scan` | 10 ms | One 7-segment digit per run (25 Hz refresh) |

Tasks due on the same tick run in the order they were added, so the buttons
are read before the FSMs use them. Only the FSM that owns the current mode does
any work; each FSM handles Button 1 for its own modes.

## Scheduler design (`scheduler.c`)

- **API:** `SCH_Add_Task(fn, delay_ms, period_ms)` returns a task ID
  (`SCH_NO_TASK_ID` if full); `period_ms = 0` means one-shot.
  `SCH_Delete_Task(id)` removes a task, including a task deleting itself.
- **Sorted delta list:** tasks are linked in due-time order, and each stores its
  delay relative to the task before it, so only the first task is decremented.
- **O(1) interrupt:** `SCH_Update()` only increments a tick counter. The task list
  is changed only in the main loop, so the interrupt can never interrupt a list
  update. Ticks counted while a task was running are applied afterwards, so none
  are lost.
- **Fixed table of `SCH_MAX_TASKS` (16) tasks:** no `malloc`.

## Buttons and modes

Same as Lab 3: Button 1 selects mode 1 → 2 → 3 → 4 → 1, Button 2 increases the
value, Button 3 saves it. In mode 1, Button 2 enters manual mode (Button 2 steps
the phase; Button 1 or 10 s idle returns to mode 1). Saving keeps
red = amber + green (red/amber saves set green = red − amber; a green save sets
red = amber + green); a save that would leave 1–99 is refused.

## Before building

- TIM2: Prescaler 7999, Period 9 (10 ms at 8 MHz) – must equal `SCH_TICK_MS`.
- Add a GPIO output labelled `LED` in CubeMX (e.g. PA0) for `Led_Blink`;
  without it the build warns and the task does nothing.
