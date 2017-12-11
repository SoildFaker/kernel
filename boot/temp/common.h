// common.h -- Defines typedefs and some global functions.
//             From JamesM's kernel development tutorials.

#ifndef COMMON_H
#define COMMON_H

void outb(u8 v, u16 port);
u8 inb(u16 port);
void outw(u16 v, u16 port);
u16 inw(u16 port);
void outl(u32 v, u16 port);
u32 inl(u16 port);

void kmemcpy(u8 *dest, const u8 *src, u32 len);
void kmemset(u8 *dest, u8 val, u32 len);

int strcmp(char *str1, char *str2);
#endif // COMMON_H
