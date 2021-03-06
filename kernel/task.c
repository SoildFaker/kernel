#include "task.h"
#include "mm.h"
#include "tools.h"
#include "string.h"
#include "common.h"
#include "debug.h"

extern void enter_user_mode();
u32 pid_now = 0;

struct task_list *running_task_head = NULL;
struct task_list *wait_task_head = NULL;
struct task_struct *current = NULL;

// System idle task
// save power
u32 task_idle(__UNUSED__ void *arg)
{
    while (1){
        hlt();
    }
    return 0;
}

void init_task()
{
    running_task_head = (struct task_list *)kmalloc(sizeof(struct task_list));
    // current is kernel thread
    current = (struct task_struct *)kmalloc(sizeof(struct task_struct));
    current->context = (struct task_context *)kmalloc(sizeof(struct task_context));

    current->state = RUNNABLE;
    current->kernel_stack = &kernel_stack;
    current->time_slice = 50;
    current->pid = pid_now++; // kernel pid is 0
    current->mm = NULL;       // do not need this for kernel
    current->tty = tty_cur;

    running_task_head->task = current;
    running_task_head->next = running_task_head;

}

// Time slice for each process would be decrease everytime schedule() called
// 
void schedule()
{
    // Current task run out time slice
    if (current->time_slice == 0) {
        current->time_slice = current->priority * 50;
        // Find next RUNNABLE
        for (;;) {
            running_task_head = running_task_head->next;
            // Next task is RUNNABLE
            if (running_task_head->task->state == RUNNABLE) {
                // Switch to next
                switch_to(running_task_head->task);
            } 
            // Return if no more task
            if (running_task_head->task == current) {
                return;
            }
        }
    } else {
        current->time_slice--;
    }
}

void switch_to(struct task_struct *next)
{
    if (current != next) {
        struct task_struct *prev = current;
        current = next;
        tty_print = next->tty;
        // Change our kernel stack over.
        set_kernel_stack((u32)next->kernel_stack+KERNEL_STACK_SIZE);
        switch_task(prev->context, current->context);
    }
}

// New task allocated
struct task_struct *alloc_task()
{
    struct task_struct *new_task = 
        (struct task_struct *)kmalloc(sizeof(struct task_struct));
    new_task->context = 
        (struct task_context *)kmalloc(sizeof(struct task_context));
    u8 *kstack = (u8 *)kmalloc(KERNEL_STACK_SIZE);
    u8 *pstack = (u8 *)kmalloc(USER_STACK_SIZE);
    assert(kstack != NULL && pstack != NULL, "kern_thread: kmalloc error");

    new_task->state = NEW;
    new_task->priority = current->priority;
    new_task->time_slice = current->priority * 50;
    new_task->kernel_stack = kstack;
    new_task->user_stack = pstack;
    new_task->parent = current;
    new_task->pid = pid_now++;
    new_task->mm = NULL;
    new_task->tty = current->tty;

    new_task->context->esp = (u32)((u32)kstack + KERNEL_STACK_SIZE);
    new_task->context->ebp = (u32)kstack;
    // let task's eflags = 0x2000 (enable interrupt)
    new_task->context->eflags = 0x200;

    // insert new task to tasklist's tail
    struct task_list *tail = running_task_head;
    assert(tail != NULL, "Must init sched!");

    while (tail->next != running_task_head) {
        tail = tail->next;
    }
    tail->next = (struct task_list *)kmalloc(sizeof(struct task_list));
    tail->next->task = new_task;
    tail->next->next = running_task_head;

    return new_task;
}

// Create kernel taskess
u32 kthread_start(u32 (*fn)(void *), struct tty *tty, u8 priority, void *arg)
{
    struct task_struct *new_task = alloc_task();

    new_task->priority = priority;
    new_task->time_slice = priority * 50;
    new_task->mm = NULL;
    new_task->tty = tty;

    u32 *stack_top = (u32 *)((u32)new_task->kernel_stack + KERNEL_STACK_SIZE);
    *(--stack_top) = (u32)arg;
    *(--stack_top) = (u32)kthread_exit;
    /**(--stack_top) = (u32)fn;*/

    new_task->context->esp = (u32)stack_top;
    new_task->context->eip = (u32)fn;

    new_task->state = RUNNABLE;
    return new_task->pid;
}

void kthread_exit()
{
    u32 val;
    asm volatile ("movl %%eax, %0\n" :"=m"(val));
    printk("Thread exited with value 0x%x\n", val);

    while(1);
}

void switch_to_user_mode()
{
    // Set up our kernel stack.
    set_kernel_stack((u32)(current->kernel_stack)+KERNEL_STACK_SIZE);

    // Set up a stack structure for switching to user mode.
    enter_user_mode();
}
