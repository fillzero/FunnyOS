tcc -I. -Ic:\tcc\include -Lc:\tcc\lib -c -ml -1 -G -O -Z -d -k- -v -vi- -wpro PC.C
pause
tcc -I. -Ic:\tcc\include -Lc:\tcc\lib -c -ml -1 -G -O -Z -d -k- -v -vi- -wpro Task.C
pause
tcc -I. -Ic:\tcc\include -Lc:\tcc\lib -c -ml -1 -G -O -Z -d -k- -v -vi- -wpro FunnyOS.c
pause
tlib my +pc+funnyos+task
pause
tlink /v /s /c c0l,funnyos,funnyos,my+cl.lib
pause
exeflat funnyos.EXE funnyos.SYS 0x60
ndisasm -o 0x600 funnyos.SYS > a.asm 
rename funnyos.SYS KERNEL.SYS
pause


nasm -dISFAT12 boot.asm -o boot.bin
pause

tcc -Ic:\tcc\include -Lc:\tcc\lib buildimg.c
buildimg.exe