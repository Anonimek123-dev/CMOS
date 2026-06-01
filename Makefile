# src C files
src_source_files := $(shell find src/ -name '*.c')
src_object_files := $(patsubst src/%.c, build/src/%.o, $(src_source_files))

# Core C files
core_c_source_files := $(shell find CMOS/Core -maxdepth 1 -name '*.c')
core_c_object_files := $(patsubst CMOS/Core/%.c, build/core/%.o, $(core_c_source_files))

# kernel C files
kernel_c_source_files := $(shell find CMOS/Core/kernel -name '*.c')
kernel_c_object_files := $(patsubst CMOS/Core/kernel/%.c, build/kernel/%.o, $(kernel_c_source_files))

# x86_64 C files
x86_64_c_source_files := $(shell find CMOS/Core/arch/x86_64 -name '*.c')
x86_64_c_object_files := $(patsubst CMOS/Core/arch/x86_64/%.c, build/x86_64/%.o, $(x86_64_c_source_files))

# x86_64 ASM files
x86_64_asm_source_files := $(shell find CMOS/Core/arch/x86_64 -name '*.asm')
x86_64_asm_object_files := $(patsubst CMOS/Core/arch/x86_64/%.asm, build/x86_64/%.o, $(x86_64_asm_source_files))

# boot ASM files
boot_asm_source_files := $(shell find CMOS/Boot -name '*.asm')
boot_asm_object_files := $(patsubst CMOS/Boot/%.asm, build/x86_64/%.o, $(boot_asm_source_files))

# drivers C files
drivers_c_source_files := $(shell find CMOS/HAL -name '*.c')
drivers_c_object_files := $(patsubst CMOS/HAL/%.c, build/x86_64/%.o, $(drivers_c_source_files))


# all x86_64 object files
x86_64_object_files := $(x86_64_c_object_files) $(x86_64_asm_object_files) $(boot_asm_object_files)

build/core/%.o: CMOS/Core/%.c
	mkdir -p $(dir $@)
	x86_64-elf-gcc -c -I CMOS -I CMOS/Core -I CMOS/HAL -ffreestanding $< -o $@

build/kernel/%.o: CMOS/Core/kernel/%.c
	mkdir -p $(dir $@)
	x86_64-elf-gcc -c -I src/ -I CMOS -I CMOS/Core -I CMOS/HAL -ffreestanding $< -o $@

build/x86_64/%.o: CMOS/Core/arch/x86_64/%.c
	mkdir -p $(dir $@)
	x86_64-elf-gcc -c -I CMOS -I CMOS/Core -I CMOS/HAL -ffreestanding $< -o $@

build/x86_64/%.o: CMOS/Core/arch/x86_64/%.asm
	mkdir -p $(dir $@)
	nasm -f elf64 $< -o $@

build/x86_64/%.o: CMOS/Boot/%.asm
	mkdir -p $(dir $@)
	nasm -f elf64 $< -o $@

build/x86_64/%.o: CMOS/HAL/%.c
	mkdir -p $(dir $@)
	x86_64-elf-gcc -c -I CMOS -I CMOS/Core -I CMOS/HAL -ffreestanding $< -o $@

build/src/%.o: src/%.c
	mkdir -p $(dir $@)
	x86_64-elf-gcc -c -I src/ -I CMOS -I CMOS/Core -I CMOS/HAL -ffreestanding $< -o $@

.PHONY: build-x86_64
build-x86_64: $(kernel_c_object_files) $(core_c_object_files) $(x86_64_object_files) $(drivers_c_object_files) $(src_object_files)
	mkdir -p dist/x86_64
	x86_64-elf-ld -n -o dist/x86_64/kernel.bin -T targets/x86_64/linker.ld \
		$(kernel_c_object_files) $(x86_64_object_files) $(drivers_c_object_files) $(src_object_files) $(core_c_object_files)
	cp dist/x86_64/kernel.bin targets/x86_64/iso/boot/kernel.bin
	grub-mkrescue /usr/lib/grub/i386-pc -o dist/x86_64/kernel.iso targets/x86_64/iso