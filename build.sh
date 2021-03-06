clear

mkdir build

echo "========================="
echo "BUILDING PROGRAMS        "
echo "-------------------------"
echo ""
echo ""
cc -c programmas/sudoku.c -o programmas/sudoku.sef -O0 -std=gnu99 -ffreestanding -m32 -mtune=i386 -Isrc/include
#../fasm/fasm ../fasm/source/SanderOS/fasm.asm


echo "========================="
echo "BUILDING KERNELMODULES"
echo "-------------------------"
echo ""
echo ""
cc -c programmas/hdd.c -o programmas/hdd.skm -std=gnu99 -ffreestanding -m32 -mtune=i386 -Isrc/include
cc -c programmas/init.c -o programmas/init.sef -std=gnu99 -ffreestanding -m32 -mtune=i386 -Isrc/include


echo "========================="
echo "BUILDING KERNEL"
echo "-------------------------"
echo ""
echo ""
nasm -felf32 src/boot.asm -o build/boot.o
nasm -felf32 src/interrupt.asm -o build/interrupt.o
#nasm -felf32 src/mouse.lib -o build/mouse.o
as src/switch.s -o build/switch.o --32 #-mtune=i386 --32
cc -c src/kernel.c -o build/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -m32 -Wextra -mtune=i386 -Isrc/include
cc -c src/screen.c -o build/screen.o -std=gnu99 -ffreestanding -O2 -Wall -m32 -Wextra -mtune=i386 -Isrc/include
cc -c src/idt.c -o build/idt.o -std=gnu99 -ffreestanding -O2 -Wall -m32 -Wextra -mtune=i386 -Isrc/include
cc -c src/ports.c -o build/ports.o -std=gnu99 -ffreestanding -O2 -Wall -m32 -Wextra -mtune=i386 -Isrc/include
cc -c src/pci.c -o build/pci.o -std=gnu99 -ffreestanding -O2 -Wall -m32 -Wextra -mtune=i386 -Isrc/include
#cc -c src/acpi.c -o build/acpi.o -std=gnu99 -ffreestanding -O2 -Wall -m32 -Wextra -mtune=i386 -Isrc/include
cc -c src/keyboard.c -o build/keyboard.o -std=gnu99 -ffreestanding -O2 -Wall -m32 -Wextra -mtune=i386 -Isrc/include
cc -c src/atapi.c -o build/atapi.o -std=gnu99 -ffreestanding -O2 -Wall -m32 -Wextra -mtune=i386 -Isrc/include
cc -c src/ata.c -o build/ata.o -std=gnu99 -ffreestanding -O2 -Wall -m32 -Wextra -mtune=i386 -Isrc/include
#cc -c src/mouse.c -o build/mouse.o -std=gnu99 -ffreestanding -O2 -Wall -m32 -Wextra -mtune=i386 -Isrc/include
cc -c src/iso.c -o build/iso.o -std=gnu99 -ffreestanding -O2 -Wall -m32 -Wextra -mtune=i386 -Isrc/include
cc -c src/elf.c -o build/elf.o -std=gnu99 -ffreestanding -O2 -Wall -m32 -Wextra -mtune=i386 -Isrc/include
cc -c src/module.c -o build/module.o -std=gnu99 -ffreestanding -O2 -Wall -m32 -Wextra -mtune=i386 -Isrc/include
cc -c src/memory.c -o build/memory.o -std=gnu99 -ffreestanding -O2 -Wall -m32 -Wextra -mtune=i386 -Isrc/include
cc -c src/filesystem.c -o build/filesystem.o -std=gnu99 -ffreestanding -O2 -Wall -m32 -Wextra -mtune=i386 -Isrc/include
cc -c src/task.c -o build/task.o -std=gnu99 -ffreestanding -O2 -Wall -m32 -Wextra -mtune=i386 -Isrc/include
cc -c src/e1000.c -o build/e1000.o -std=gnu99 -ffreestanding -O2 -Wall -m32 -Wextra -mtune=i386 -Isrc/include
ld -n -T src/linker.ld -o myos.bin -O2 -nostdlib build/boot.o build/kernel.o build/ata.o build/keyboard.o build/pci.o build/atapi.o build/idt.o build/interrupt.o build/screen.o build/ports.o build/iso.o build/elf.o build/module.o build/memory.o build/filesystem.o build/task.o build/switch.o build/e1000.o -m elf_i386
if grub-file --is-x86-multiboot myos.bin; then
  	

	echo "========================="
	echo "CREATING ISO"
	echo "-------------------------"
	echo ""
	echo ""
	mkdir -p isodir/boot/grub
	mkdir -p isodir/modules
	mkdir -p isodir/programs
	cp myos.bin isodir/boot/myos.bin
	cp src/grub.cfg isodir/boot/grub/grub.cfg
	cp programmas/hdd.skm -a isodir/modules/hdd.skm
	cp programmas/init.sef -a isodir/programs/init.sef
	cp programmas/sudoku.sef -a isodir/programs/sudoku.sef
	cp ../fasm/source/SanderOS/fasm.o isodir/programs/fasm.sef
	grub-mkrescue -o myos.iso isodir
	
#	echo "========================="
#	echo "CREATING USB LEGACY"
#	echo "-------------------------"
#	echo ""
#	echo ""
#	nasm -O0 -w+orphan-labels -f bin -o src/bootloader/usb/stage2.bin src/bootloader/usb/stage2.asm
#	nasm -O0 -w+orphan-labels -f bin -o src/bootloader/usb/bootload.bin src/bootloader/usb/bootload.asm
#	cat src/bootloader/usb/bootload.bin myos.bin > src/bootloader/usb/image.bin
#	mkdir innerloop
#	sudo dd if=src/bootloader/usb/image.bin of=/dev/sdb
#	sudo mount /dev/sdb innerloop
#	sudo cp src/bootloader/usb/stage2.bin innerloop/stage2.bin
#	sudo cp myos.bin innerloop/kernel.bin
#	sudo umount innerloop
#	rm -r innerloop

	echo "========================="
	echo "STARTING EMULATOR"
	echo "-------------------------"
	echo ""
	echo ""
	qemu-system-i386 -cdrom myos.iso
else
  echo the file is not multiboot
fi


echo "========================="
echo "CLEANUP"
echo "-------------------------"
echo ""
echo ""
rm -r build



echo "========================="
echo "PUSH TO GIT"
echo "-------------------------"
echo ""
echo ""
git add --all .
git commit -a -m "auto"
git push origin master



echo "========================="
echo "FINISHED"
echo "-------------------------"
echo ""
echo ""
