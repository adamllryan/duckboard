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
