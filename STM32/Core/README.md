# Lab 3 – Traffic light (STM32F103C6)

## Buttons (Lab 3 handout, section 5.1)

| Button | Role |
|---|---|
| Button 1 (PB0) | Select mode: 1 → 2 → 3 → 4 → 1 (from manual: back to mode 1) |
| Button 2 (PB1) | Modes 2–4: increase the value (1–99; hold: +1 after 1 s, then every 0.5 s). Mode 1: enter manual mode. Manual: next phase |
| Button 3 (PB2) | Modes 2–4: save the value |

## Modes

| Mode | Lights | 7-segment (left / right) |
|---|---|---|
| 1 Normal | R_G → R_A → G_R → A_R, timed | road 1 / road 2 countdown |
| 2 Modify red | All red LEDs blink at 2 Hz | `02` / value |
| 3 Modify amber | All amber LEDs blink at 2 Hz | `03` / value |
| 4 Modify green | All green LEDs blink at 2 Hz | `04` / value |
| Manual (extra) | Phase stepped by Button 2; 10 s idle → mode 1 | `00` / phase 01–04 |

The cycle needs red = amber + green. Saving red or amber sets green = red − amber;
saving green sets red = amber + green. A save that would put any value outside
1–99 is refused, and the display shows the stored value again.
Changing mode with Button 1 without pressing Button 3 discards the edit.

## Code structure

| File | Purpose |
|---|---|
| `software_timer.c` | Named software timers (`setTimer`, `isTimerExpired`), durations in ms |
| `button.c` | 3-sample debounce in the interrupt; per-loop event snapshot |
| `traffic_light.c` | Table-driven light outputs (`setRoadColor`, `setBothRoads`) |
| `display7seg.c` | 4-digit multiplexing with blanking (10 ms per digit) |
| `fsm.c` | Mode FSM (Button 1), phase→lights table, display output |
| `fsm_automatic.c` | Mode 1 sub-FSM |
| `fsm_setting.c` | Modes 2–4 sub-FSM |
| `fsm_manual.c` | Manual sub-FSM |
| `timer.c` | TIM2 callback: `button_reading()` + `timerRun()` every 10 ms |

The tick is `TIMER_CYCLE` in `software_timer.h`; it must match TIM2
(Prescaler 7999, Period 9 at 8 MHz → 10 ms).
