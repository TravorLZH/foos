# Foos
Another OS project because my TravorOS's code is too messy

## Plan
These are my little goals to make this OS

- [x] Bootloader
- [x] GDT
- [x] C kernel
- [x] Interrupts
- [ ] IRQs
- [ ] Timer
- [ ] Keyboard
- [x] VGA

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
