# Lab 4 – Scheduler on top of the Lab 3 traffic light

TIM2 interrupt every 10 ms: `SCH_Update()` (counts one tick).
Main loop: `SCH_Dispatch_Tasks()`.
The TIM2 callback is in `main.c`.

| Task | Period |
|---|---|
| `timer_Run`, `getKeyInput` | 10 ms |
| `fsm_manual`, `fsm_automatic`, `fsm_setting` | 10 ms |
| `display_scan` | `SCAN_PERIOD_MS` (20 ms) |

Tasks due on the same tick run in the order they were added, so buttons are read before the FSMs.

`scheduler.c`: tasks are kept in a list sorted by due time, each storing its delay relative to
the task before it, so a tick only changes the first task. The interrupt only counts ticks;
the list is only changed in the main loop. 16 fixed slots, no `malloc`. A period of 0 runs a
task once; `SCH_Delete_Task` also works on the task that is running.

The traffic light files (`global`, `software_timer`, `button`, `traffic_light`,
`fsm_automatic`, `fsm_manual`, `fsm_setting`) and button/mode behaviour are the same as Lab 3.
TIM2: Prescaler 7999, Period 9 (10 ms at 8 MHz) = `SCH_TICK_MS`.

## Simulation speed (Proteus)

- `SCAN_PERIOD_MS` in `traffic_light.h` sets how long each digit stays on
  (10 ms = steadiest, 20 ms default, 30–50 ms = fastest but visibly flickering).
- LEDs and segments are written with one `BSRR` write per port, so all pins of a
  group switch at the same instant.
- In Proteus, setting the STM32's *Simulation mode* to PERFORMANCE also helps.
