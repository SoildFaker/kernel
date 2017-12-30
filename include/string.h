#ifndef __STRING__
#define __STRING__

#include "common.h"
#include "display.h"

void memcpy(u8 *dest, const u8 *src, u32 len);
void memset(u8 *dest, u8 val, u32 len);
int strcmp(const char *str1, const char *str2);
char *strcpy(char *dest, const char *src);
char *strcat(char *dest, const char *src);
int strlen(const char *src);

#endif // INCLUDE_STRING_H_
