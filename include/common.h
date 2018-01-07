#ifndef __COMMON__
#define __COMMON__

#define NULL 0
#define __UNUSED__    __attribute__((unused))

#define __init        __attribute__((section(".init.text")))
#define __init_data   __attribute__((section(".init.data")))
#define __packed      __attribute__((packed))

/* this panic just get into dead loop */
#define  PANIC(info)                       \
{                                          \
  printk("KERNEL PANIC: %s\n", info);    \
  while(1) ;                               \
}                                          \

#define assert(x, info) \
  do { \
    if (!(x)) { \
      PANIC(info); \
    } \
  } while (0)

typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned long  u32;

void kmain(void);

static inline void cli(void)
{
  asm volatile("cli");
}

static inline void hlt(void)
{
  asm volatile("hlt");
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
