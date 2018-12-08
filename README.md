# Foos
[![AppVeyor Branch](https://ci.appveyor.com/api/projects/status/github/TravorLZH/foos?svg=true)](https://ci.appveyor.com/project/TravorLZH/foos)
[![Github Release](https://img.shields.io/github/tag/TravorLZH/foos.svg)](https://github.com/TravorLZH/foos/releases)
[![Github License](https://img.shields.io/github/license/TravorLZH/foos.svg)](LICENSE)

FOOS stands for `FOOS's Overcoming Operating System`

## Plan
These are my little goals to make this OS

- [x] Bootloader
- [x] 32-bit Segmentation (GDT and stuff)
- [x] C kernel
- [x] Interrupts (IDT and stuff)
- [x] Interrupt Requests
- [x] Programmable Interval Timer
- [x] Keyboard
- [x] VGA
- [x] Virtual Memory Management
- [x] Physical Memory Management (Frames, Page Frame Allocator)
- [x] Porting `liballoc` (malloc)
- [x] Hardware Abstraction Layer (`dev_write()`, `dev_read()`, `dev_open()`,
`dev_close()`, and `dev_ioctl()`)
- [x] RAM Drive
- [ ] Floppy Disk Controller
- [ ] Loadable Kernel Module
- [x] Virtual File System
- [ ] Multitasking
- [ ] User Space

## Prerequisites
This OS is targeting x86 processors, so you need to have `i386-elf` toolchain
installed before building.

## Build
Change `Makefile`'s `CC`, `AS`, and `LD` to your toolchain and run:
```shell
$ make dep
$ make
```

> This OS can successfully be built on Termux without libgcc

You can also type `make run` to play the OS if you have `qemu-system-i386`
installed.
## Images
This OS currently supports only floppy images that perfectly fits into a 1.44 MB 3.5" Floppy. You may treat it as a hard disk image as well
