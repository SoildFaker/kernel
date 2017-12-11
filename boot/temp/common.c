#include <tinx/types.h>
#include <boot/common.h>

/* Basic port I/O */
inline void outb(u8 v, u16 port)
{
	asm volatile("outb %0,%1" : : "a" (v), "dN" (port));
}

inline u8 inb(u16 port)
{
	u8 v;
	asm volatile("inb %1,%0" : "=a" (v) : "dN" (port));
	return v;
}

inline void outw(u16 v, u16 port)
{
	asm volatile("outw %0,%1" : : "a" (v), "dN" (port));
}

inline u16 inw(u16 port)
{
	u16 v;
	asm volatile("inw %1,%0" : "=a" (v) : "dN" (port));
	return v;
}

inline void outl(u32 v, u16 port)
{
	asm volatile("outl %0,%1" : : "a" (v), "dN" (port));
}

inline u32 inl(u16 port)
{
	u32 v;
	asm volatile("inl %1,%0" : "=a" (v) : "dN" (port));
	return v;
}

// Copy len bytes from src to dest.
void kmemcpy(u8 *dest, const u8 *src, u32 len)
{
  const u8 *sp = (const u8 *)src;
  u8 *dp = (u8 *)dest;
  for(; len != 0; len--) *dp++ = *sp++;
}

// Write len copies of val into dest.
void kmemset(u8 *dest, u8 val, u32 len)
{
  u8 *temp = (u8 *)dest;
  for ( ; len != 0; len--) *temp++ = val;
}

// Compare two strings. Should return -1 if 
// str1 < str2, 0 if they are equal or 1 otherwise.
int strcmp(char *str1, char *str2) {
    int i = 0;
    int failed = 0;
    while(str1[i] != '\0' && str2[i] != '\0') {
      if(str1[i] != str2[i]) {
        failed = 1;
        break;
      }
      i++;
    }
    // why did the loop exit?
    if( (str1[i] == '\0' && str2[i] != '\0') || (str1[i] != '\0' && str2[i] == '\0') )
      failed = 1;
  
    return failed;
}

