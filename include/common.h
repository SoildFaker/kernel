// common.h -- Defines typedefs and some global functions.
//             From JamesM's kernel development tutorials.

#ifndef __COMMON__
#define __COMMON__

/* this panic just get into dead loop */
#define  PANIC(Expression)                       \
  {                                              \
    kprint("KERNEL PANIC: %s\n", Expression);    \
    while(1) ;                                   \
  }                                              \

typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned long  u32;

#define NULL 0

static inline void cli(void)
{
  asm volatile("cli");
}

static inline void sti(void)
{
  asm volatile("sti");
}

static inline void stosb(void *addr, int data, int cnt)
{
  asm volatile("cld; rep stosb" :
      "=D" (addr), "=c" (cnt) :
      "0" (addr), "1" (cnt), "a" (data) :
      "memory", "cc");
}

static inline void insl(u16 port, void *addr, int cnt)
{
  asm volatile("cld; rep insl" :
      "=D" (addr), "=c" (cnt) :
      "d" (port), "0" (addr), "1" (cnt) :
      "memory", "cc");
}

/* Basic port I/O */
static inline void outb(u16 port, u8 v)
{
	asm volatile("outb %0,%1" : : "a" (v), "dN" (port));
}

static inline u8 inb(u16 port)
{
	u8 v;
	asm volatile("inb %1,%0" : "=a" (v) : "dN" (port));
	return v;
}

static inline void outw(u16 port, u16 v)
{
	asm volatile("outw %0,%1" : : "a" (v), "dN" (port));
}

static inline u16 inw(u16 port)
{
	u16 v;
	asm volatile("inw %1,%0" : "=a" (v) : "dN" (port));
	return v;
}

static inline void outl(u16 port, u32 v)
{
	asm volatile("outl %0,%1" : : "a" (v), "dN" (port));
}

static inline u32 inl(u16 port)
{
	u32 v;
	asm volatile("inl %1,%0" : "=a" (v) : "dN" (port));
	return v;
}

#endif // __COMMON__
