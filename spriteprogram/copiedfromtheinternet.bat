@echo off
del *.nes

cc65 -Oi friend.c --add-source -v
ca65 friend.s -v
ca65 reset.s -v
ld65 -C nes.cfg --obj friend.o reset.o nes.lib -o stupid.nes -v

del *.o
pause