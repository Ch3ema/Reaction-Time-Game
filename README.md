# Reaction Time Duel

A two-player reaction time game on an Arduino Uno. An LED and buzzer
fire after a randomized delay, and the first player to hit their button
wins the round. Best of 5, with live times and running score on a 16x2 LCD.

<img width="300" alt="IMG_2337" src="https://github.com/user-attachments/assets/e7f8d661-c1dc-4065-bace-af71122afc78" />



## How it works
- A randomized 1 to 4 second delay runs before the start signal, seeded
  from an unconnected analog pin so the sequence differs on every power-up
- `millis()` is captured the instant the signal fires and again on each
  button press, giving reaction times in milliseconds
- Button presses are read as falling edges on `INPUT_PULLUP` pins, so a
  held button only registers once
- Presses faster than 30 ms are rejected as anticipation or switch bounce
  rather than real reactions
- Round winner, per-round times, and cumulative score are pushed to the
  LCD, with an LED and buzzer victory pattern

## Hardware
| Part | Detail |
|---|---|
| MCU | Arduino Uno |
| Display | 16x2 HD44780 LCD, 4-bit mode |
| Input | 2x momentary push button, internal pull-ups |
| Output | 3x LED, piezo buzzer |
| Build | Breadboard, jumper wires |

Pin map is defined at the top of the sketch.

## Typical results
Human reaction times land around 150 to 250 ms. Screenshot above shows
a round won at 126 ms.

## Things I'd change
- Replace the blocking `delay()` calls in the LED and buzzer patterns
  with a `millis()` based state machine, so input polling never stops
- Add a round timeout so one player failing to press doesn't stall the game
- Award the round on a tie instead of silently skipping the point
