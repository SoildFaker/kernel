#ifndef DEBUG_H
#define DEBUG_H

/* this panic just get into dead loop */
#define  PANIC(info)                       \
{                                          \
    printk("KERNEL PANIC: %s\n", info);    \
    while(1) ;                             \
}                                          \

#define  ERROR(info)                       \
{                                          \
    printk_color(COLOR_RED, COLOR_BLACK, "%s\n", info);    \
}                                          \

#define assert(x, info) \
    do { \
        if (!(x)) { \
            PANIC(info); \
        } \
    } while (0)

#endif
