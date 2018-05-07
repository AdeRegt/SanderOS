#include "system.h"
 //
// M U L T I T A S K I N G
// TutorialURL: http://wiki.osdev.org/Kernel_Multitasking

extern void switchTask(unsigned long a,unsigned long b);

static Task *runningTask;
static Task mainTask;
static Task otherTask;

static void otherMain() {
    printf("Hello multitasking world!"); // Not implemented here...
    yield();
}

void initTasking() {
    // Get EFLAGS and CR3
    asm volatile("movl %%cr3, %%eax; movl %%eax, %0;":"=m"(mainTask.regs.cr3)::"%eax");
    asm volatile("pushfl; movl (%%esp), %%eax; movl %%eax, %0; popfl;":"=m"(mainTask.regs.eflags)::"%eax");
    register long counter asm("esp");
   mainTask.regs.esp = counter;
    createTask(&otherTask, otherMain, mainTask.regs.eflags, (unsigned long*)mainTask.regs.cr3);
    mainTask.next = (struct Task *)&otherTask;
    otherTask.next = (struct Task *)&mainTask;
    runningTask = &mainTask;
}

void createTask(Task *task, void (*main)(), unsigned long flags, unsigned long *pagedir) {
    task->regs.eax = 0;
    task->regs.ebx = 0;
    task->regs.ecx = 0;
    task->regs.edx = 0;
    task->regs.esi = 0;
    task->regs.edi = 0;
    task->regs.eflags = flags;
    task->regs.eip = (unsigned long) main;
    task->regs.cr3 = (unsigned long) pagedir;
    task->regs.esp = (unsigned long) mainTask.regs.esp;//allocPage() + 0x1000; // Not implemented here
    task->next = 0;
}

void yield() {
    Task *last = runningTask;
    runningTask = (Task *)runningTask->next;
    switchTask((unsigned long)&last->regs, (unsigned long)&runningTask->regs);
}
