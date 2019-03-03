# FOOS's Overcoming Operating System
[![AppVeyor Branch][appveyor]](https://ci.appveyor.com/project/TravorLZH/foos)
[![Github Tag][tag]](https://github.com/TravorLZH/foos/releases)
[![Github License][license]](LICENSE)

FOOS still remains as a **hobbyist research purpose** operating system, so use
it _wisely_.

## Currently implemented

* Bootloader
* 32-bit C kernel
* Interrupts and Interrupt Requests
* Programmable Interval Timer
* Teletype Device (Both printing and keyboard
* Memory Management (Paging and Page Frame Allocator)
* Hardware Abstraction Layer (`dev_write()`, `dev_read()`, etc.)
* Simple `malloc()` from scratch that is `free()`able
* Virtual File System
* RAM Drive

## Still Stuck
* Floppy Disk Controller
* Loadable Kernel Module
* Multitasking
* User Space

## Build
The process of building FOOS is not that complicated since it has not been a
huge, powerful, and charming operating system.
> However, without the support of a decent home-brewed toolchain, the difficulty
will be increased

### Prerequisites
This OS is targeting x86 processors, so you need to have `i386-elf` toolchain
installed before building.
> **libgcc** is optional in this case

Change `Makefile`'s `CC`, `AS`, and `LD` to your toolchain and run:
```shell
$ make dep
$ make
```
You can also type `make run` to play the OS if you have `qemu-system-i386`
installed.

## Images
Harddisk images are supported:
* *bootdisk.img:* 1st hard disk channel (DAP is used to load _system_)
* *ramdisk.img:* 2nd hard disk channel

## RAM Disk
FOOS has two filesystem models. One is made from scratch that does not support
subdirectories, and the other is implemented through an ancient standard (TAR
filesystem).

### RAMFS
```c
struct rd_header {
	uint16_t signature;
	uint16_t nfiles;
};

struct rd_fileheader {
	uint32_t magic;
	char name[32];
	uint32_t offset;
	uint32_t size;
};
```
> Detailed declarations can be found in [`include/foos/ramfs.h`][1]

The first structure is the header of the whole ramdisk image, and it tells
how many files this filesystem has. After this header, an array of fileheaders
follows. Most of the fields are self-explanatory, but `offset` tells where the
content of a files starts, so `offset + size` is where the file ends

### TAR filesystem

This filesystem is apparently more powerful than the one shown above, in which
it supports subdirectory and more unix fields. The structure of its header
should look like this:
```c
struct tar_header {
	char filename[100];
	char mode[8];
	char uid[8];
	char gid[8];
	char size[12];
	char mtime[12];
	char chksum[8];
	char type;	// Identify what type the file is
	char linkname[100];
	char ustar[6];
	char ustar_ver[2];
	char owner[32];
	char group[32];
	char padding[183];
	char data[];	// To access the content if it is a file
};
```
I am not going to explain this, but you can find more information in
[Wikipedia][2]

[appveyor]: https://ci.appveyor.com/api/projects/status/github/TravorLZH/foos?svg=true
[tag]: https://img.shields.io/github/tag/TravorLZH/foos.svg
[license]: https://img.shields.io/github/license/TravorLZH/foos.svg
[1]: include/foos/ramfs.h
[2]: https://en.wikipedia.org/wiki/Tar_(computing)
