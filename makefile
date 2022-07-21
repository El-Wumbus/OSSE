strlib_sources = $(wildcard src/kernel/strlib/*.c++)
cross-compilerxx = $(shell printf "${HOME}/opt/cross/bin/i686-elf-g++")

default:build-iso

build: clean
	mkdir -p dist/kernel/strlib
	mkdir -p dist/boot/

	for file in $(strlib_sources); do \
		$(cross-compilerxx) -c $$file -o dist/kernel/strlib/$$(scripts/replace_file_extention.py $$(basename $$file) .o) -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti; \
	done

	$(cross-compilerxx) -c src/kernel/strlib/strlen.c++ -o dist/kernel/strlib/strlen.o -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti
	$(cross-compilerxx) -c src/kernel/kernel.c++ -o dist/kernel/kernel.o -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti
	i686-elf-as src/boot/boot.s -o dist/boot/boot.o
	$(cross-compilerxx) -T src/boot/linker.ld -o dist/decatorOS.bin -ffreestanding -O2 -nostdlib  $$(ls dist/kernel/strlib/*.o) dist/boot/boot.o dist/kernel/kernel.o -lgcc

build-iso: build
	grub-file --is-x86-multiboot dist/decatorOS.bin
	mkdir -p dist/isodir/boot/grub
	cp dist/decatorOS.bin dist/isodir/boot/decatorOS.bin
	cp src/boot/grub.cfg dist/isodir/boot/grub/grub.cfg
	grub-mkrescue -o dist/decatorOS.iso dist/isodir
run: build-iso
	qemu-system-i386 -cdrom dist/decatorOS.iso

clean:
	rm -fr dist
