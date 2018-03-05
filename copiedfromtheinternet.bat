cc65 -Oi idiot.c --add-source
ca65 reset.s
ca65 idiot.s
ld65 -C nes.cfg -o idiot.s reset.o nes.lib
@REM del *.o