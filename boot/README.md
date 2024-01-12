# Boot process
Boot stands for bootstrap in shorthand. For us it means starting up the computer


## General steps of boot
1. Power ON
2. Load BIOS/UEFI from NVRAM/ROM // Need to verify it is ROM or not, Yes its rom, but modern system have flash memory, so we can write something into it
3. Probe for hardware
4. Select boot device (disk, network)
5. Identify EFI system partition
6. Load boot loader (GRUB2)
7. Determin which kernel to boot
8. Load the kernel
9. Instantiate the data structures required for grub
10. Start init/systemd as PID 1
11. Execute startup scripts
12. Running system

ROM is immutable memory device, if you want to change it change the chip 🤓


### System firmware
When the system is powered on, the cpu is forced to run a hardwired boot code stored in rom
This knows all about the devices that are connected to the motherboard. This lets configure the devices to hide from the 
OS or not

During the normal bootstrapping process the system firmware probes for hardware, disks and runs a simple set of 
health checks, and then looks for the next stage of bootstrapping process.


### BIOS(Basic Input/Output System) vs UEFI(Unified Extensible Firmware Interface)

- **BIOS**: Slower, less secure, in older systems
- **UEFI**: Faster, more secure, in modern system

Virtualized environment tends to use BIOS

#### Legacy BIOS
This assumes that the boot devices start with a record called MBR (Master Boot Record).
The MBR includes both a first stage boot loader and a primitive disk partitioning table. The amount of space available
for the MBR is very less, less than 512 Bytes 🤏. 
So what it can do is load and run the second stage boot loader.

1. The bios looks for the device which can be booted (Checks, hard drive, usb, floppy, etc).
2. Then checks for the MBR from the sector 0 of the disk
3. Using the MBR it runs thes second stage boot loader code and that also has information about the disk partitions
4. The space available for the first stage boot loader is very less as mentioned
5. The second stage boot loader has a good amount of space available as compared to the first stage. \
For historic reasons the first parition of the disk doesn't begin until the 64th block. So this space is available for the boot loader \
So it has around 32KB space
6. The boot loader goes throught the partition from the MBR and loads the boot loader from that location

#### UEFI
UEFI specification uses GPT (GUID Partition Table, Globally unique identifier) for managing disk partition.
UEFI also understands FAT(File Allocation Table) file system. So now we have a new concept of EFI system partition.

So in runtime it consults GPT to find the ESP and it then reads the configured target application directly from the file in ESP
and executes it.

Since ESP is just a FAT filesystem, it can be mounted, read, written and maintained by any of the operating system.
So UEFI technically uses a boot loader less process. The boot target can be unix/linux kernel that has been configured for direct UEFI
loading.

But in practice, most of the system uses a boot loader just to provide a backward compatibility for OS which supports only BIOS

UEFI saves the path to load from the ESP as a configuration parameter, usually it assumes the /boot/efi/bootx64.efi by default

#### Boot Loaders
Boot loaders are different from both UEFI/BIOS and OS kernel loaders. Its also seperate from the distinct inital boot block in BIOS system
Another task of book loader is to marshall the kernel arguments.


#### Grub (GRand Unified Boot Loader)
Free BSD has its own boot loader, Grub is boot loader used majorly in linux distributions world.
Grub is capable of booting free BSD also


It is not recommended to edit directly the grub config but instead generate new grub config using update-grub2 or equivalent
You can edit the grub config by making a file in `/etc/grub.d` folder and adding the configuration

You can define the thing that should be started on boot using the GRUB_CMDLINE
🤓 You can tell kernel to start /bin/bash as a init process, Dont know why in the world you would do that 🤪
But generally the Kernel argument is path for the systemd process or init process

#### Systemd and init process
Run right complement of services and daemons at a given time
Reponsibilities of init
- Single User mode, only minimal set of filesystem is mounted, no services are running, and a root shell is started on console.
- Multi User mode, in which all the filesystem is mounted, all the required services are running, and a GUI interface.
- Server mode, similar to multiuser but with no GUI

Three impementation of init
- Traditional init
- Free BSD based init
- And the one and only **SYSTEMD**

## Traditional Init
System modes in traditional init have something called run levels, Most run levels are denoted by a number or a digit
Most system have a standard fix number of configuration that never changes. These configuration in turn run a second tier shell scripts 
that actual does the job
The second layer scripts maintain a third layer of daemon and system-specific scripts, which are cross linked to run-level specification
directories that indicates which service should run at what run level
Everything is sequential in the traditional init world.
I will not get conflicts if I dont work in parallel 🤓
**If it is not broken dont fix it**

## Systemd 
Systemd takes all the init features formerly implemented with sticky tapes, shell script hacks, and the sweat of administrators and 
formalizes them into a unified field of theory of services

So what systemd is cool apart from all the arguments, But traditional init has still a role to play when either installation footprint
is small or doesn't need systemd's advanced process management

Systemd is not a single daemon. [This](https://0pointer.de/blog) blog posts tells that systemd generates 69 (This is 
true, dont judge me 😄) different binaries

Systemd heavily depends on linux kernel so it is not compatible with Free BSD or for sake any UNIX like systems, atleast for next 5 yrs

### Units and unit files
An entity managed by systemd is known generically as a unit.
A unit can be a "service, socket, watched filesystem path, startup target, or a wormwhole into alternate universe"

So now lets look at the unit file syntax 😜

```service
[Unit]
Description=fast remote file copy program daemon
ConditionPathExists=/etc/rsyncd.conf
[Service]
ExecStart=/usr/bin/rsync --daemon --no-detach
[Install]
WantedBy=multi-user.target
```
If you figured it out,yeah that it looks similar to the MS-DOS .ini file format, this is the reason why systemd 
recieved so much hate

Unit files can live in different places. /usr/lib/systemd/system is the main place where packages deposit their unit files
on some systems it is /lib/systemd/system instead. And your custom configuration can go in /etc/systemd/system
There's also a /run/systemd/system thats a scratch area for transient units 🤯, So much to take on

So systemd looks for all these directories, so they're pretty much same, in case of conflict /etc/systemd gets higher
priority

```service
[Unit]
Description=The nginx HTTP and reverse proxy server
After=network.target remote-fs.target nss-lookup.target

[Service]
Type=forking
PIDFile=/run/nginx.pid
ExecStartPre=/usr/bin/rm -f /run/nginx.pid
ExecStartPre=/usr/sbin/nginx -t
ExecStart=/usr/sbin/nginx
ExecReload=/bin/kill -s HUP $MAINPID
KillMode=process
KillSignal=SIGQUIT
TimeoutStopSec=5
PrivateTmp=true

[Install]
WantedBy=multi-user.target
```

This is a bit complex service of systemd. The startup command is expected to exit and the daemon continues to work in bg

So for god for sake reason you want to update the existing unit files DONT DO IT, JUST BE NICE, If you want to shoot yourself
then please go ahead.

The correct way to override the existing unit files configuration is to create a folder called unit-file.d/
and create some shit.conf file. Then override your configs

We cannot override Install section of a service file

**One important thing you should keep in mind that systemd allows systemd allows to repeat a key multiple times in a 
single unit file, they form a list and are all active simultaneously, if you want to really override the conf then you
should first set the key to a empty value first**

Systemd has many architectural implications, and adopting it is not simple for teams building linux distributions

Systemd has some glue left to be backwards compatible with trad init


Easter EGG 🥚: In CentOS or Red Hat Linux still run the /etc/rc.d/rc.local script at boot time if we configure it to
be executable

In theory we can still use the hack scripts, but act like a grown up man and use systemd like a chad 🪴




## Shutting down physical systems

The halt command performs essetial duties required to shutdown the system, halt logs shutdown, kills non essential 
process, flushes cached filesystem blocks to disk, and then halts kernel

Reboot does the same thing but except it doesn't halt the system instead reboot

shutdown is a layer of halt and reboot, shutdown does nothing of technical value than halt or reboot.
Feel free to ignore if you dnt have a multi user system


## 3 Godly rules of linux debuggin
**📓 Remember with great power comes great responsibility 📓**
1. Dont just debug, restore the system to a well known good state,
2. Bring the system up, to just get a root shell and debug interactively
3. Boot a seperate image, and mount the sick filesystem and investigate from there
