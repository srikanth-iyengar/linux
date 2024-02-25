# C for kernel development

1. Function calls
2. Code size

## inline functions
Usage of inline functions kind of embeds your functiosn directly to the caller site removing the actual function call

```C
static inline void woof(int a);
```

## inline assembly
The gcc compiler enables embedding assembly instructions in otherwise normal C functions. This feature is of course based on the system architecture

The Linux kernel is written in a mixture of C and assembly, with assembly relegated to low-level architecture and fast path code
```C
unsigned int low, high;
asm volatile("rdtsc" : "=a" (low), "=d" (high));
/* low and high now contain the lower and upper 32-bits of the 64-bit tsc */
```

## Branch annotation
GCC has a built-in directive that optimizes conditional branching as either very likely or unlikely to happen

```C
if(likely(error)) {
    // do something since this error will occur majority of the time
}
```

```C
if(unlikely(error)) {
    // do something because your most trusted API failed to do the shit
}
```


## No memory protection
When in user-space you try to access a illegal memory address the kernel is there to prevent a catastrophhic failure, but in case of kernel space no one is there to protect it

**_Kernel memory is not pageable_**

## No (Easy) use of Floating Point
When a user-space uses floating point instruction, kernel manages the transition from integer to floating point, but when its time for kernel to do that it varies by architecture

If you want to use floating point numbers in kernel you to store and floating point registers by yourself

**Short answer: Don't do it**

## Small, Fixed-Size stack
User-space get away with allocating large data strucutres of thousands and millions of elements of variables on stack.
This is illegal in this kernel area, kernel stack is neither small nor big it is exactly 4KB or 8KB in 32bit architectures and 16KB in 64 bit architectures

## Synchronization and Concurrency
- Linux is a preemptive multitasking operating system. Processes are scheduled and rescheled at whimps of kernel's process scheduler.
- Linux supports SMP(Symmetrical Multiprocessing). Multiple processes can access a shared resource
- The Linux kernel is preemptive.Therefore, without protection, kernel code can be preempted in favor of different code that then accesses the same resource.
Typical solutions are semaphores, spin locks.
