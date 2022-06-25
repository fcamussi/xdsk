AS = sdasz80
CC = sdcc -mz80


all: crt0msx_msxdos.rel crt0msx_msxdos_advanced.rel putchar.rel getchar.rel heap.rel msxdisk.rel


crt0msx_msxdos.rel: crt0msx_msxdos.s
	$(AS) -o crt0msx_msxdos.s

crt0msx_msxdos_advanced.rel: crt0msx_msxdos_advanced.s
	$(AS) -o crt0msx_msxdos_advanced.s

putchar.rel: putchar.s
	$(AS) -o putchar.s

getchar.rel: getchar.s
	$(AS) -o getchar.s

heap.rel: heap.c heap.h
	$(CC) -c heap.c

msxdisk.rel: msxdisk.c msxdisk.h
	$(CC) -c msxdisk.c

clean:
	rm -f *.asm *.lst *.rel *.sym

