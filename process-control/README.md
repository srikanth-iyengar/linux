# Process

## Components of process
A process consists of address space & set of data structures within the kernel

Address space is set of pages that kernel has marked for the process

These pages contains the code and library of the process which is executing, process variables, its stack, and various extra information

Kernels internal data structure record various pieces of information about each process

- Address space map
- Current status of the process
- Execution priority of process
- Information about the resources and process has used
- Information about the files and network ports the process has opened
- The process signal mask ( record of signals which are blocked )

Thread is a execution context within a process, every process has atleast one thread and can have many also

## PID: Process Identifier
The kernel assigns a ID for each of the process to uniquely identify the process

Most commands and syscalls require you the specify the pid when manipulating the process

PIDs are assigned as the processes are created in order

Linux defines the concept of namespace, which further restrict process's ability to see and affect each other

Container implementation use this to keep the processes segregated.

Side effect of this is that process might appear to have different pids depending on the namespace of the observer

## PPID: parent PID

Neither UNIX or LINUX has a system call to create a process running a particular program
Instead it is a two step process
1. A process must clone itself and create a new one.
2. The clone can exchange the program its running to a different one

## UID and EUID: real and effective user ID
- A process's UID is the user identification number of the person who created it, or it is a COPY value of the parent process. Usually the owner or the superuser only can manipulate a process
- A process's EUID is effective User ID, this is used to tell what all resources and files a process has permission to access at any given moment
- Most systems track 'Saved UID' which is a copy of the process's EUID at the point at which the process starts to execute
- Linux also defines a non standard FSUID process parameter that controls the determination of filesystem permissions

## GID and EGID: real and effective group ID
- The GID is the group identification number of a process
- EGID and GID relate in same way as how UID and EUID relate
- Determination of access permissions normally take account into the EGID and the supplemental group list
- The only time GID is used significantly is when the process creates new files, depending on how file system permissions have been set the process may adopt the default GID of the creating process.

## Control terminal
Most non daemon process have a terminal associated with it. This provides a default linkage of in/out/err paths.

It also distributes signals to process in response to key press like `<C-c>` `<C-z>`, etc.

Real terminals are rare outside these days. They live in the form of psuedo terminals, which are still used widely through out UNIX and LINUX systems.

## Life cycle of process
To create a new process the parent calls copies itself using the **fork** system calls, and that copy is largely identical to the parent.

**fork** has a unique property of returning two different values for parent and child.

For child process it returns zero, and for parent it returns the value of the pid of child

Since the two process are otherwise identical they should look into the return values and figure out what they are supposed to do

The child often uses one the **exec** families of routines to begin the execution of new system call

When the system boots kernel starts several process. The most notable of these is init or systemd, the roll no 1, I mean the pid 1

You can find more of systemd at [Boot](../boot/README.md) 

When a process complete its executiong it calls a **exit** routine to notify the kernel that it is ready to die.

It supplies exit code that tells why its exiting

Before a dead process be allowed to disappear completely the kernel requires that its death be acknowledged by the parent, which the parent doesn it with **wait** syscall. Parent recieves copy of child's exit code.

This scheme works fine if the parent process dies after the child process. But if the parent process dies before the chidl process, then the kernel manages to assign the systemd as a parent for the process, which potentially 

## Signals
Signals are process level interrupt requests. About 30 different kinds of defined, they are used in a variety of ways

PS: core dump is a copy of process's memory image, which is sometimes useful in debugging

When a signal is recived of of the two things happen
1. If the reciveing process has a designated handler routine for that particular signal, the the handler is called with the speicif context in which signal was delivered.
2. If not then the kernel takes some default action on behalf of the process. the default action varies from signal to signal

Specifying handler for a routine for a signal is known as catching the signal, just a jargon

Obviously when the handler completes, execution restarts from the point at which the signal was rcvd 🤯

To prevent the signals from arriving, programs can request that they have either blocked or ignored

A signal that is previously ignore is simly discarded and the signal which was previously blocked is queued for delivery, but kernel doesnt required process to to act until the signal is unblocked

| #   | Signal Name         | Description               | Default | Can Catch? | Can Block? | Dump Core? |
| --- | -------------------- | ------------------------- | ------- | ---------- | ---------- | ---------- |
| 1   | HUP                  | Hangup                    | Yes     | Yes        | Yes        | No         |
| 2   | INT                  | Interrupt                 | Yes     | Yes        | Yes        | No         |
| 3   | QUIT                 | Quit                      | Yes     | Yes        | Yes        | No         |
| 9   | KILL                 | Kill                      | No      | No         | No         | Yes        |
| 10  | BUS                  | Bus error                 | Yes     | Yes        | Yes        | No         |
| 11  | SEGV                 | Segmentation fault        | Yes     | Yes        | Yes        | No         |
| 15  | TERM                 | Software termination      | Yes     | Yes        | Yes        | No         |
| 17  | STOP                 | Stop                      | No      | No         | Yes        | No         |
| 18  | TSTP                 | Keyboard stop             | Yes     | Yes        | No         | No         |
| 19  | CONT                 | Continue after stop       | Yes     | No         | Yes        | No         |
| 28  | WINCH                | Window changed            | Yes     | Yes        | No         | No         |
| 30  | USR1                 | User-defined #1           | Yes     | Yes        | Yes        | No         |
| 31  | USR2                 | User-defined #2           | Yes     | Yes        | Yes        | No         |

The BUS and SEGV signals are also error signals. These are more common signals. When a program crashes it is one of these signals that brings the process down

The signals are of not diagnostic value. Both of them indicate improper access of memory

The signals KILL and STOP cannot be caught, blocked, or ignored. **KILL** destroys the process and discards from memory.

**STOP** suspends the process until a CONT signal is sent to the process

**TSTP** is a "soft" version **STOP** signal that might be best described as a request to stop.
It is the signal generally generated by the terminal driver when you press **<C-z>** (You know this if you use vim 🤓)

Programs that catch this signal cleanup their state and send itself a **STOP** signal

Ignoring **TSTP** makes process not to terminale using keyboard

## KILL vs INT vs TERM vs HUP vs QUIT
- **KILL** is unblockable and terminates a process at kernel level. A process never actually recives this signal
- **INT** is sent by the terminal driver when user preccess <C-c> Its a request to terminate the current operation. Simple program should quit (if they catch) or kill themselves (default behauviour)
- **TERM** is a request to eliminate execution completly. Its expected that the process will clean up its state and exit
- **HUP** has two common interpretations, its understood as a reset request for daemon process. If daemon is capable of reading the configuration file and adjusting to the new changes without restarting
-- **HUP** signals are generated by terminal drivers to clean up ("kill") the process attached to the terminal


## Process and thread states
Process can be suspended using the STOP signal and continue execution with CONT signal. However the state of being suspended applies to the process as a whole and the runnable threads inherit it

Even in normal situations thread has to wait for kernel to complete some background before they continue execution.

Like if a thread wants to read a file, the kernel has to bring the required disk blocks in the process address space. During this time the process enters a partial sleep state

We may come across situations where a process will be shown in sleep state, since sleep is a thread level attribute and it is a bit deceptive

Process show in a sleep state when all of its thread are sleep

Sometimes process/thread enter into a uninterruptible sleep state. This state is usually transient and is not opserved in ps D in STAT column

Process which enter this state cannot be killed you have to fix the underlying problem or reboot the system

Some examples involve problem in the NFS server mounted with some `hard` option


## ps to monitor process's
ps is a command line utility for developers not `sysadmin`. ps shows required information about process

ps accepts command line argument with or without dashes but assign different interpretations to those forms. ps -a is not same as ps a

Few command line set which you will use frequently

ps aux, a -> show all process, u -> user oriented output format, x -> show process that dont even have a contro terminal
ps lax, l -> long form of output format ax have the same meaning

ps lax might be efficient than aux since the uid is not translated to actual username

| Field         | Contents                                    |
| ------------- | ------------------------------------------- |
| USER          | Usernames of the process owner              |
| PID           | Process ID                                  |
| %CPU          | Percentage of the CPU this process is using |
| %MEM          | Percentage of real memory process is using  |
| VSZ           | Virtual size of process                     |
| RSS           | Resident size of the process                |
| TTY           | Control terminal ID                         |
| STAT          | Current Process Satus   R=Runnable D=In interruptible sleep, S=Sleeping T=Traced or stopped, Z=Zombie Additional flags: W=process is swapped out <= process has higher prority N=process has lower priority L=some packages are locked in core s=process is session leader|

## NICE and RENICE
The niceness of a process is a numeric hint to the kernel about how the process should be treated in relation to other process contending for CPU

How nice you are wrt to other users in the system

High niceness low priority of the process, Low or negatice niceness means high priority

The range of niceness value in lnux ranges from -20 to 19 in Free BSD systems it is -20 to +20

Unless you intentinally play with niceness the process inherits niceness from the parent

By default owner of the process can increase the niceness of process but cannot lower it

nice and renice are command line tools to manipulate the CPU scheduling priorities for I/O ionice should be used

nice when creating a process renice after the process is created

## All you need to know about /proc
/proc is a psuedo file system in *nix systems where the kernel exposes informatio about processes, the ps and top command reads this file system

Despite the name `/proc` the information is not limited to process information - a variety of status information and kernel statistics are also present. You can modify some parameters by writing to `/proc`

Process specific informatino is divided into directories 

The useful ones are given below

| File  | Contents |
| ----- | -------- |
| cgroup | The control group to which the process belongs |
| cmd | The command or the program the process is executing |
| cmdline | Complete command line of the process `null` selerated
| cwd | Symbolic link to process's current working directory |
| environ | The environment variables of the process `null` seperated |
| exe | Symbolic link to the file being executed |
| fd | A subdirectory containing symbolic links to the open fds |
| fdinfo | Subdirectory containing information about each of the open file descriptors | 
| maps | Memory mapping information (shared segments, libs, etc |
| ns | Subdirectory with links to the namespace used by the process | 
| root | Symbolic link to the process's root directory | 
| stat | General purpose information about process ( best decoded by ps ) |
| statm | memory usage information |
