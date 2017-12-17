#include "init.h"
#include "keyboard.h"
#include "tools.h"
#include "common.h"

u8 kbdus[128] = {
  0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	  /* 9 */
  '9', '0', '-', '=', '\b',	                      /* Backspace */
  '\t',			                                  /* Tab */
  'q', 'w', 'e', 'r',	                              /* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	  /* Enter key */
  0,			                                      /* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ':', /* 39 */
  '\'', '`',   0,		                              /* Left shift */
  '\\', 'z', 'x', 'c', 'v', 'b', 'n',			      /* 49 */
  'm', ',', '.', '/',   0,				          /* Right shift */
  '*',
  0,	                                              /* Alt */
  ' ',	                                          /* Space bar */
  0,	                                              /* Caps lock */
  0,	                                              /* 59 - F1 key ... > */
  0,   0,   0,   0,   0,   0,   0,   0,
  0,	                                              /* < ... F10 */
  0,	                                              /* 69 - Num lock*/
  0,	                                              /* Scroll Lock */
  0,	                                              /* Home key */
  0,	                                              /* Up Arrow */
  0,	                                              /* Page Up */
  '-',
  0,	                                              /* Left Arrow */
  0,
  0,	                                              /* Right Arrow */
  '+',
  0,	                                              /* 79 - End key*/
  0,	                                              /* Down Arrow */
  0,	                                              /* Page Down */
  0,	                                              /* Insert Key */
  0,	                                              /* Delete Key */
  0,   0,   0,
  0,	                                              /* F11 Key */
  0,	                                              /* F12 Key */
  0,	                                              /* All other keys are undefined */
};

u8 kbdus_upper[128] = {
  0,  27, '!', '@', '#', '$', '%', '^', '&', '*',	  /* 9 */
  '(', ')', '_', '+', '\b',	                      /* Backspace */
  '\t',			                                  /* Tab */
  'Q', 'W', 'E', 'R',	                              /* 19 */
  'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',	  /* Enter key */
  0,			                                      /* 29   - Control */
  'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', /* 39 */
  '\"', '~',   0,		                              /* Left shift */
  '\\', 'Z', 'X', 'C', 'V', 'B', 'N',			      /* 49 */
  'M', '<', '>', '?',   0,				          /* Right shift */
  '*',
  0,	                                              /* Alt */
  ' ',	                                          /* Space bar */
  0,	                                              /* Caps lock */
  0,	                                              /* 59 - F1 key ... > */
  0,   0,   0,   0,   0,   0,   0,   0,
  0,	                                              /* < ... F10 */
  0,	                                              /* 69 - Num lock*/
  0,	                                              /* Scroll Lock */
  0,	                                              /* Home key */
  0,	                                              /* Up Arrow */
  0,	                                              /* Page Up */
  '-',
  0,	                                              /* Left Arrow */
  0,
  0,	                                              /* Right Arrow */
  '+',
  0,	                                              /* 79 - End key*/
  0,	                                              /* Down Arrow */
  0,	                                              /* Page Down */
  0,	                                              /* Insert Key */
  0,	                                              /* Delete Key */
  0,   0,   0,
  0,	                                              /* F11 Key */
  0,	                                              /* F12 Key */
  0,	                                              /* All other keys are undefined */
};

static int shift_state = 0 ;

void keyboard_callback()
{
  u8 ch;
  u8 code = inb(0x60); //read the data buffer
  if(code == 0x2a || code == 0x36)
    shift_state = 1;
  else if(code == 0xaa || code == 0xb6 )
    shift_state = 0;

  /* If the top bit of the byte we read from the keyboard is
  *  set, that means that a key has just been released */
  if (code & 0x80) {
    /* You can use this one to see if the user released the
    *  shift, alt, or control keys... */
  } else {
    /* Here, a key was just pressed. Please note that if you
    *  hold a key down, you will get repeated key press
    *  interrupts. */
    if(code == SHIFT_L || code == SHIFT_R) return;
    ch = shift_state? kbdus_upper[code] : kbdus[code];
    display_putc(ch, COLOR_GREEN, COLOR_BLACK);
  }
}

void init_keyboard()
{
  register_interrupt_handler(IRQ1, (interrupt_handler_t)&keyboard_callback);
  irq_enable(1);
}
