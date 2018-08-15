# Foos
Another OS project because my TravorOS's code is too messy

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
- [ ] RAM Drive
- [ ] Floppy Disk Controller
- [ ] Loadable Kernel Module
- [ ] Virtual File System
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
You can also type `make run` to play the OS if you have `qemu-system-i386`
installed.
## Images
This OS currently supports only floppy images that perfectly fits into a 1.44 MB 3.5" Floppy. You may try it on hard disk
