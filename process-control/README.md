# Process

## Components of process
A process consists of address space & set of data structures within the kernel
Address space is set of pages that kernel has marked for the process
These pages contains the code and library of the process which is executing, process variables, its stack, and various
extra information


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

Real terminals are rare outside these days. They live in the form of psuedo terminals, which are still used widely
through out UNIX and LINUX systems.

## Life cycle of process
To create a new process the parent calls copies itself using the **fork** system calls, and that copy is largely identical
to the parent.

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
