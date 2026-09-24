.PHONY: run clean

CFLAGS = -Wall -Wextra -O2 -ffreestanding -fno-stack-protector -fno-PIC -m64 -mcmodel=kernel -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -mno-80387
LDFLAGS = -T src/linker.ld -nostdlib -static

build/redlineos.iso: build/kernel limine.conf limine/limine
	mkdir -p build/iso_root/boot/limine build/iso_root/EFI/BOOT
	cp build/kernel build/iso_root/boot/
	cp limine.conf limine/limine-bios.sys limine/limine-bios-cd.bin limine/limine-uefi-cd.bin build/iso_root/boot/limine/
	cp limine/BOOTX64.EFI build/iso_root/EFI/BOOT/
	xorriso -as mkisofs -R -r -J -b boot/limine/limine-bios-cd.bin -no-emul-boot -boot-load-size 4 -boot-info-table -hfsplus -apm-block-size 2048 --efi-boot boot/limine/limine-uefi-cd.bin -efi-boot-part --efi-boot-image --protective-msdos-label build/iso_root -o build/redlineos.iso
	./limine/limine bios-install build/redlineos.iso

build/kernel: build/kernel.o src/linker.ld
	ld $(LDFLAGS) -o build/kernel build/kernel.o

build/kernel.o: src/kernel.c 
	mkdir -p build
	gcc -c src/kernel.c -o build/kernel.o $(CFLAGS)

limine/limine:
	curl -fL https://github.com/Limine-Bootloader/Limine/releases/download/v12.9.0/limine-binary.tar.gz | tar -xz
	mv limine-binary limine
	$(MAKE) -C limine

run: build/redlineos.iso
	qemu-system-x86_64 -cdrom build/redlineos.iso -no-reboot -display none -monitor stdio

clean:
	rm -rf build

