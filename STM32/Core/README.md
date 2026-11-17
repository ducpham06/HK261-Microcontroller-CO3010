# Lab 3 – Traffic light (STM32F103C6)

TIM2 interrupt every 10 ms: `getKeyInput()` + `timer_Run()`.
Main loop: `fsm_automatic()`, `fsm_setting()`, `fsm_manual()`, and `display_scan()` every 10 ms.
Each FSM returns at once unless it owns the current mode, and enters the next mode by calling its `enter` function.

| Button | Role |
|---|---|
| B1 (PB0) | Mode 1 → 2 → 3 → 4 → 1 (manual → mode 1) |
| B2 (PB1) | Modes 2–4: value + 1 (hold: after 1 s, every 0.5 s). Mode 1: manual mode. Manual: next phase |
| B3 (PB2) | Modes 2–4: save the value |

| Mode | Lights | Display |
|---|---|---|
| 1 Normal | R_G → R_A → G_R → A_R | road 1 / road 2 countdown |
| 2 / 3 / 4 | Red / amber / green blink at 2 Hz | `02`/`03`/`04` + value |
| Manual | Stepped by B2, 10 s idle → mode 1 | `00` + phase 01–04 |

Saving keeps red = amber + green (red or amber → green = red − amber; green → red = amber + green); a save outside 1–99 is refused.

| File | Contents |
|---|---|
| `global.c` | Mode, phase, durations, countdowns |
| `software_timer.c` | `setTimer`, `isTimerExpired`, `timer_Run` |
| `button.c` | 3-sample debounce, B2 auto-repeat |
| `traffic_light.c` | `setLights`, `setPhaseLights`, 7-segment buffer and scanning |
| `fsm_automatic.c`, `fsm_setting.c`, `fsm_manual.c` | One FSM per mode group |
| `timer.c` | TIM2 callback |

TIM2: Prescaler 7999, Period 9 (10 ms at 8 MHz).
