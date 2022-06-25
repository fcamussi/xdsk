CC = sdcc -mz80 -I../backend-msxdos
OPTS = --code-loc 0x0178 --data-loc 0 --no-std-crt0
LIBS = ../backend-msxdos/crt0msx_msxdos_advanced.rel\
       ../backend-msxdos/putchar.rel\
       ../backend-msxdos/getchar.rel\
       ../backend-msxdos/heap.rel\
       ../backend-msxdos/msxdisk.rel

xdsk.com: xdsk.c
	$(CC) $(OPTS) $(LIBS) xdsk.c
	../bin/hex2bin xdsk.ihx
	mv xdsk.bin xdsk.com

clean:
	rm -f *.asm *.lst *.rel *.sym *.map *.ihx *.lk *.noi xdsk.com

