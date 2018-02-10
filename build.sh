mkdir build
nasm -felf32 src/boot.asm -o build/boot.o
cc -c src/kernel.c -o build/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -m32 -Wextra -mtune=i386 -Isrc/include
cc -c src/screen.c -o build/screen.o -std=gnu99 -ffreestanding -O2 -Wall -m32 -Wextra -mtune=i386 -Isrc/include
cc -c src/idt.c -o build/idt.o -std=gnu99 -ffreestanding -O2 -Wall -m32 -Wextra -mtune=i386 -Isrc/include
ld -n -T src/linker.ld -o myos.bin -O2 -nostdlib build/boot.o build/kernel.o build/idt.o build/screen.o -m elf_i386
if grub-file --is-x86-multiboot myos.bin; then
  	echo multiboot confirmed
	mkdir -p isodir/boot/grub
	cp myos.bin isodir/boot/myos.bin
	cp src/grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o myos.iso isodir
	qemu-system-i386 -cdrom myos.iso
else
  echo the file is not multiboot
fi
rm -r build
git add --all .
git commit -a -m "auto"
git push origin master
