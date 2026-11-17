# Lab 3 – Traffic light (STM32F103C6)

TIM2 interrupt every 10 ms (callback in `main.c`): `getKeyInput()` + `timer_Run()`.
Main loop: `fsm_automatic()`, `fsm_setting()`, `fsm_manual()`, and `display_scan()` every `SCAN_PERIOD_MS`.
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
| `fsm_automatic.c/.h` | Mode 1 |
| `fsm_setting.c/.h` | Modes 2–4 |
| `fsm_manual.c/.h` | Manual mode |

TIM2: Prescaler 7999, Period 9 (10 ms at 8 MHz).

## Simulation speed (Proteus)

- `SCAN_PERIOD_MS` in `traffic_light.h` sets how long each digit stays on
  (10 ms = steadiest, 20 ms default, 30–50 ms = faster Proteus but visible flicker).
- LEDs and segments are written with one `BSRR` write per port, so all pins of a
  group switch at the same instant (fewer recalculations in Proteus).
- Setting the STM32's *Simulation mode* to PERFORMANCE in Proteus also helps.
