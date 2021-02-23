#!/bin/bash
export PREFIX="$HOME/opt/cross"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"
flags=""
qemuf=""
debug=""
while true; do
    read -p "Debug Build? [y/n]" yn
    case $yn in
        [Yy]* ) flags="-D DEBUG"; qemuf="-serial file:./serial.txt"; debug="-Debug"; break;;
        [Nn]* ) break;;
        * ) echo "Not an answer.";;
        esac
done
mkdir Output
i686-elf-gcc -c ./ASM/int.c -o ./Output/int.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra $flags -I"./Headers/"
i686-elf-gcc -c ./ASM/inoutb.c -o ./Output/inoutb.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra $flags -I"./Headers/"
i686-elf-gcc -c ./Boot/boot.c -o ./Output/entry.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra $flags -I"./Headers/"
i686-elf-gcc -c ./IO/text.c -o ./Output/text.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra $flags -I"./Headers/"
i686-elf-gcc -c ./IO/keyboard.c -o ./Output/key.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra $flags -I"./Headers/"
i686-elf-gcc -c ./IO/serial.c -o ./Output/serial.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra $flags -I"./Headers/"
i686-elf-gcc -c ./IO/vga.c -o ./Output/vga.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra $flags -I"./Headers/"
i686-elf-gcc -c ./IO/power.c -o ./Output/power.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra $flags -I"./Headers/"
i686-elf-gcc -c ./CMD/cmd.c -o ./Output/cmd.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra $flags -I"./Headers/"
i686-elf-gcc -c ./CMD/terminal.c -o ./Output/terminal.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra $flags -I"./Headers/"
nasm -felf32 ./ASM/int.asm -o ./Output/intasm.o
nasm -felf32 ./Boot/boot.asm -o ./Output/boot.o
cd Output
i686-elf-gcc -T ../Boot/linker.ld -o ../J-OS$debug.bin -I"../Headers/" -ffreestanding -O2 -nostdlib int.o intasm.o inoutb.o entry.o boot.o text.o key.o vga.o serial.o cmd.o power.o terminal.o -lgcc
cd ..
if grub-file --is-x86-multiboot ./J-OS$debug.bin; then
    echo Compile succeeded
    mkdir -p ~/isodir/boot/grub
    cp ./J-OS$debug.bin ~/isodir/boot/J-OS.bin
    cp ./grub.cfg ~/isodir/boot/grub/grub.cfg
    grub-mkrescue -o ./ISO/J-OS$debug.iso ~/isodir
    rm -r ~/isodir
else
    echo Compile failed
fi
qemu-system-i386 -kernel ./J-OS$debug.bin -d guest_errors $qemuf
rm serial.txt
rm -r Output