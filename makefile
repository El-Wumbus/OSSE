default:build-iso

build: clean
	mkdir -p dist
	i686-elf-g++ -c src/kernel/kernel.c++ -o src/kernel/kernel.o -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti
	i686-elf-as src/boot/boot.s -o src/boot/boot.o
	i686-elf-g++ -T src/boot/linker.ld -o decatorOS.bin -ffreestanding -O2 -nostdlib src/boot/boot.o src/kernel/kernel.o -lgcc

build-iso: build
	grub-file --is-x86-multiboot decatorOS.bin
	mkdir -p dist/isodir/boot/grub
	cp decatorOS.bin dist/isodir/boot/decatorOS.bin
	cp src/boot/grub.cfg dist/isodir/boot/grub/grub.cfg
	grub-mkrescue -o dist/decatorOS.iso dist/isodir
run: build-iso
	qemu-system-i386 -cdrom dist/decatorOS.iso

clean:
	rm -fr src/kernel/kernel.o src/boot/boot.o decatorOS.bin decatorOS.iso dist
