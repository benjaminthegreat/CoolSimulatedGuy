@echo off
del *.nes

cc65 -Oi idiot.c --add-source -v
ca65 idiot.s -v
ca65 reset.s -v
ld65 -C nes.cfg --obj idiot.o reset.o nes.lib -o asdf.nes -v

del *.o
pause