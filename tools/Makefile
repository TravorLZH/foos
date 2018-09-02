# Makefile for FOOS utility tools
CC=gcc

all:	mkdisk

mkdisk:	mkdisk.c ../include/dev/ramdisk.h
	$(CC) -g -o $@ $<

clean:
	rm -f mkdisk
