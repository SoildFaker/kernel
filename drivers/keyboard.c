#include "init.h"
#include "drivers/keyboard.h"
#include "tools.h"
#include "drivers/tty.h"
#include "common.h"

static struct keymap us_keymap = {
  //normal keys
  {
    /* first row - indices 0 to 14 */
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    /* second row - indices 15 to 28 */
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', //Enter key
    /* 29 = Control, 30 - 41: third row */
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    /* fourth row, indices 42 to 54, zeroes are shift-keys*/
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
    '*',
    /* Special keys */
    0,    // ALT - 56
    ' ', // Space - 57
    0,    // Caps lock - 58
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // F1 to F10 - 59 to 68
    0, // Num lock - 69
    0, // Scroll lock - 70
    0, // Home - 71
    72, // Up arrow - 72
    0, // Page up - 73
    '-',
    0, // Left arrow - 75
    0,
    0, // Right arrow -77
    '+',
    0, // End - 79
    80, // Dowm arrow - 80 
    0, // Page down - 81
    0, // Insert - 82
    0, // Delete - 83
    0, 0, 0,
    0, // F11 - 87
    0, // F12 - 88
    0, // All others undefined
  },
  // caps
  {
    /* first row - indices 0 to 14 */
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    /* second row - indices 15 to 28 */
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
    /* 29 = Control, 30 - 41: third row */
    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'', '`',
    /* fourth row, indices 42 to 54, zeroes are shift-keys*/
    0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', '/', 0, '*',
    /* Special keys */
    0,   // ALT - 56
    ' ', // Space - 57
    0,   // Caps lock - 58
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // F1 to F10 - 59 to 68
    0, // Num lock - 69
    0, // Scroll lock - 70
    0, // Home - 71
    0, // Up arrow - 72
    0, // Page up - 73
    '-',
    0, // Left arrow - 75
    0,
    0, // Right arrow -77
    '+',
    0, // End - 79
    0, // Dowm arrow - 80
    0, // Page down - 81
    0, // Insert - 82
    0, // Delete - 83
    0, 0, 0,
    0, // F11 - 87
    0, // F12 - 88
    0, // All others undefined
  },
  // shift
  {
    /* first row - indices 0 to 14 */
    0, 27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
    /* second row - indices 15 to 28 */
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
    /* 29 = Control, 30 - 41: third row */
    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~',
    /* fourth row, indices 42 to 54, zeroes are shift-keys*/
    0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0, '*',
    /* Special keys */
    0,   // ALT - 56
    ' ', // Space - 57
    0,   // Caps lock - 58
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // F1 to F10 - 59 to 68
    0, // Num lock - 69
    0, // Scroll lock - 70
    0, // Home - 71
    0, // Up arrow - 72
    0, // Page up - 73
    '-',
    0, // Left arrow - 75
    0,
    0, // Right arrow -77
    '+',
    0, // End - 79
    0, // Dowm arrow - 80
    0, // Page down - 81
    0, // Insert - 82
    0, // Delete - 83
    0, 0, 0,
    0, // F11 - 87
    0, // F12 - 88
    0, // All others undefined
  },
  // control_map
  {
    29, // Ctrl
    56, // Alt
    0,  // AltGr
    42, // left Shift
    54, // right Shift
    58, // Caps lock
    70, // Scroll lock
    69  // Num lock
  },
  // 键盘的控制键信息初始化为 0 
  0
};

void keyboard_callback()
{
  u8 scancode = inb(0x60);

  struct keymap *layout = &us_keymap;

  // keyboard handler 
  // scancode & RELEASED_MASK means the key has been break
  if (scancode & RELEASED_MASK) {
    u32 i;
    // Clean control state
    for (i = 0; i < 5; i++) {
      if(layout->control_map[i] == (scancode & ~RELEASED_MASK)) {
        layout->controls &= ~(1 << i);
        return;
      }
    }
    // key pressed
  } else {
    u32 i;
    // Check control set
    for (i = 0; i < 8; i++) {
      // If pressed is control key set control bit to 1
      // otherwise set to 0
      if (layout->control_map[i] == scancode) {
        if (layout->controls & 1 << i) {
          layout->controls &= ~(1 << i);
        } else {
          layout->controls |= (1 << i);
        }
        return;
      }
    }
    u8 *scancodes = layout->scancodes;

    // If shift pressed, switch to shift code layout
    if ((layout->controls & (LSHIFT | RSHIFT)) && !(layout->controls & CONTROL)) {
      scancodes = layout->shift_scancodes;
    }
    // If Caps Lock on , switch layout to upper case
    if ((layout->controls & (CAPSLOCK)) && !(layout->controls & CONTROL)) {
      scancodes = layout->capslock_scancodes;
    }
    // Switch tty
    if ((layout->controls & (ALT)) && !(layout->controls & CONTROL)){
      if (scancode < TTY_NUMBER+2) switch_tty(&tty[scancode-2]);
      return;
    }
    display_putc(COLOR_BLACK, COLOR_GREEN, tty_cur, scancodes[scancode]);
  }
}

void init_keyboard()
{
  // register keyboard interrupt handler
  register_interrupt_handler(IRQ1, (interrupt_handler_t)&keyboard_callback);
  // enable keyboard interrupt
  irq_enable(1);
}
