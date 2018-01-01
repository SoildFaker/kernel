#ifndef __KEYBOARD__
#define __KEYBOARD__

#include "common.h"

#define CONTROL         0x1
#define ALT             0x2
#define ALTGR           0x4
#define LSHIFT          0x8
#define RSHIFT          0x10
#define CAPSLOCK        0x20
#define SCROLLLOCK      0x40
#define NUMLOCK         0x80

// Key Break Mask
#define RELEASED_MASK 0x80

struct keymap {
  u8 scancodes[128];
  u8 capslock_scancodes[128];
  u8 shift_scancodes[128];
  u8 control_map[8];
  u8 controls;
};
void init_keyboard();

#endif
