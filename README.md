# Duckboard
QMK keymap for doodboard duckboard macropad that adds multiple uses for the macropad including rotary encoder modes, game mode, macro keys, etc

This contains the keymap and rules files for the duckboard, you need to compile it and flash it to the board using QMK toolbox





Features: 
-4 layers
  -Base (Layer 0 holds copy macros, layer controls, play/pause, scroll mode toggle, and will be able to record and play custom macros in the future)
  -Func (Layer 1 holds function keys up to 12 and can do the same as layer 0)
  -Macro(Layer 2 has layer/scroll controls, copy paste, and has custom buttons that can be mapped in other software for custom use)
  -Game (Layer 3 [only toggled by pressing rotary encoder down] requires 90 degree rotation of the macropad and emulates the left side of a keyboard for video games)
  
-4 scroll wheel modes
  -Window (can scroll through open windows using alt-tab)
  -History Scrubbing (can scrub edit history)
  -Page Scrolling (can scroll up and down page)
  -Volume Scrolling (volume can scroll up and down)

TODO: 
  -Add recordable macros
  -Fix game mode
  -Add splash screen for game mode
  -make oled display look nicer

Here's what the layers look like:

Layer 1: 
|                  | TO(2)       |      |       |            |
|------------------|-------------|------|-------|------------|
|                  | TO(1)       |      |       |            |
|                  | TG(0)       |      |       |            |
| Toggle Game mode | Scroll Mode |      |       |            |
| Toggle OLED      | CUT         | COPY | PASTE | PLAY/PAUSE |

Layer 2

|                  | TO(2)       |  F12 | F8    | F4         |
|------------------|-------------|------|-------|------------|
|                  | TO(1)       | F11  | F7    | F3         |
|                  | TG(0)       | F10  | F6    | F2         |
| Toggle Game mode | Scroll Mode | F9   | F5    | F1         |
| Toggle OLED      | CUT         | COPY | PASTE | PLAY/PAUSE |

|                  | TO(2)       |  M12 | M8    | M4         |
|------------------|-------------|------|-------|------------|
|                  | TO(1)       | M11  | M7    | M3         |
|                  | TG(0)       | M10  | M6    | M2         |
| Toggle Game mode | Scroll Mode | M9   | M5    | M1         |
| Toggle OLED      | CUT         | COPY | PASTE | PLAY/PAUSE |


















