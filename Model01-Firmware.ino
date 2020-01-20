// -*- mode: c++ -*-
// Copyright 2016 Keyboardio, inc. <jesse@keyboard.io>
// See "LICENSE" for license details

#ifndef BUILD_INFORMATION
#define BUILD_INFORMATION "locally built"
#endif

#include "Kaleidoscope.h"
#include "Kaleidoscope-EEPROM-Settings.h"
#include "Kaleidoscope-EEPROM-Keymap.h"
#include "Kaleidoscope-FocusSerial.h"
#include "Kaleidoscope-LEDControl.h"
#include "Kaleidoscope-Macros.h"
#include "Kaleidoscope-LEDEffect-BootGreeting.h"
#include "Kaleidoscope-LEDEffect-SolidColor.h"
#include "Kaleidoscope-LEDEffect-Chase.h"
#include "Kaleidoscope-LED-Stalker.h"
#include "Kaleidoscope-HostPowerManagement.h"
#include "Kaleidoscope-MagicCombo.h"
#include "Kaleidoscope-USB-Quirks.h"

enum
{
  PRIMARY,
  FUNCTION
};

enum
{
  MACRO_LOCK,
  MACRO_SLEEP
};

/* This comment temporarily turns off astyle's indent enforcement
 *   so we can make the keymaps actually resemble the physical key layout better
 */
// *INDENT-OFF*

KEYMAPS(
    [PRIMARY] = KEYMAP_STACKED(Key_Escape, Key_1, Key_2, Key_3, Key_4, Key_5, Key_Quote,
                               Key_Backtick, Key_Q, Key_W, Key_E, Key_R, Key_T, Key_Tab,
                               Key_PageUp, Key_A, Key_S, Key_D, Key_F, Key_G,
                               Key_PageDown, Key_Z, Key_X, Key_C, Key_V, Key_B, Key_LeftBracket,
                               Key_LeftControl, Key_Backspace, Key_LeftGui, Key_LeftShift,
                               ShiftToLayer(FUNCTION),

                               Key_Equals, Key_6, Key_7, Key_8, Key_9, Key_0, Key_Minus,
                               Key_Enter, Key_Y, Key_U, Key_I, Key_O, Key_P, Key_Backslash,
                               Key_H, Key_J, Key_K, Key_L, Key_Semicolon, Key_Quote,
                               Key_RightBracket, Key_N, Key_M, Key_Comma, Key_Period, Key_Slash, Key_Minus,
                               Key_RightShift, Key_LeftAlt, Key_Spacebar, Key_RightControl,
                               ShiftToLayer(FUNCTION)),

    [FUNCTION] = KEYMAP_STACKED(Key_LEDEffectNext, Key_F1, Key_F2, Key_F3, Key_F4, Key_F5, Key_CapsLock,
                                Key_Tab, ___, ___, ___, ___, ___, ___,
                                Key_Home, ___, ___, ___, ___, ___,
                                Key_End, Key_PrintScreen, Key_Insert, ___, ___, ___, ___,
                                ___, ___, ___, ___,
                                ___,

                                M(MACRO_LOCK), Key_F6, Key_F7, Key_F8, Key_F9, Key_F10, Key_F11,
                                Key_Enter, ___, Key_UpArrow, ___, ___, ___, Key_F12,
                                Key_LeftArrow, Key_DownArrow, Key_RightArrow, ___, ___, ___,
                                M(MACRO_SLEEP), Consumer_Mute, Consumer_VolumeDecrement, Consumer_VolumeIncrement, Consumer_PlaySlashPause, Key_Backslash, ___,
                                ___, ___, Key_Enter, ___,
                                ___))

/* Re-enable astyle's indent enforcement */
// *INDENT-ON*
static kaleidoscope::plugin::LEDSolidColor solidRed(160, 0, 0);
static kaleidoscope::plugin::LEDSolidColor solidOrange(140, 70, 0);
static kaleidoscope::plugin::LEDSolidColor solidYellow(130, 100, 0);
static kaleidoscope::plugin::LEDSolidColor solidGreen(0, 160, 0);
static kaleidoscope::plugin::LEDSolidColor solidBlue(0, 70, 130);
static kaleidoscope::plugin::LEDSolidColor solidIndigo(0, 0, 170);
static kaleidoscope::plugin::LEDSolidColor solidViolet(130, 0, 120);

void toggleLedsOnSuspendResume(kaleidoscope::plugin::HostPowerManagement::Event event)
{
  switch (event)
  {
  case kaleidoscope::plugin::HostPowerManagement::Suspend:
    LEDControl.disable();
    break;
  case kaleidoscope::plugin::HostPowerManagement::Resume:
    LEDControl.enable();
    break;
  case kaleidoscope::plugin::HostPowerManagement::Sleep:
    break;
  }
}

void hostPowerManagementEventHandler(kaleidoscope::plugin::HostPowerManagement::Event event)
{
  toggleLedsOnSuspendResume(event);
}

static void toggleKeyboardProtocol(uint8_t combo_index)
{
  USBQuirks.toggleKeyboardProtocol();
}

USE_MAGIC_COMBOS({.action = toggleKeyboardProtocol,
                  // Left Fn + Esc + Shift
                  .keys = {R3C6, R2C6, R3C7}});

const macro_t *macroAction(uint8_t macroIndex, uint8_t keyState)
{
  switch (macroIndex)
  {

  case MACRO_LOCK:
    return MACRODOWN(I(25),
                     D(LeftAlt), T(Space), U(LeftAlt),
                     T(L), T(O), T(C), T(K),
                     T(Enter));
  case MACRO_SLEEP:
    return MACRODOWN(I(25),
                     D(LeftAlt), T(Space), U(LeftAlt),
                     T(S), T(L), T(E), T(E), T(P),
                     T(Enter));
  }
  return MACRO_NONE;
}

KALEIDOSCOPE_INIT_PLUGINS(
    EEPROMSettings,
    EEPROMKeymap,
    Focus,
    FocusSettingsCommand,
    FocusEEPROMCommand,
    BootGreetingEffect,
    LEDControl,
    LEDOff,
    Macros,
    solidRed, solidGreen, solidBlue,
    StalkerEffect,
    HostPowerManagement,
    MagicCombo,
    USBQuirks);

void setup()
{
  Kaleidoscope.setup();
  StalkerEffect.variant = STALKER(Haunt, (CRGB(0, 220, 220)));
  EEPROMKeymap.setup(5);
}

void loop()
{
  Kaleidoscope.loop();
}
