# CH32V003 Flash Memory Storage 

Tired of losing those precious user config settings every time the power blinks? Fear not, you can now save them without needing an extra flash chip!

This library lets you tame the flash memory in your CH32V003 microcontroller. It's like a digital safe for your non-volatile settings, but remember, this safe has a limited number of uses (10,000 to be exact). For heavy-duty use, consider getting an external flash chip for added durability.

## About CH32V003

Your trusty cheap CH32V003 microcontroller comes with a 16K flash memory, and we can set aside a portion of it to store data that pops up during runtime, like calibration values or your favorite user settings.

The catch? The flash has limited write cycles (flash endurance), and you have to do some prep work before writing your values.

But for stuff that doesn't change much, it's a perfect fit!

Get all the nitty-gritty details and usage instructions in `ch32v003_flash.h`.

This code utilizes the ch32v003fun project for unlocking the raw power of this microcontroller. Refer to their GitHub for installation instructions.

## TL;DR

- Store important stuff in your CH32V003's main flash memory
- During boot, call `flash_set_latency()` once to get things rolling.
- Functions without "OB" in their names work with the main flash (OB indicates an option byte)
- To alter the main flash:
  1. Unlock it with `flash_unlock()`.
  2. Erase page(s) using `flash_erase_64b()`. Sorry, no smaller erasures allowed!
  3. Program your values with `flash_program_16()`.
  4. Lock the flash using `flash_lock()`.
- To tweak option bytes:
  1. Unlock the flash with `flash_unlock()`.
  2. Unlock option bytes with `flash_OB_unlock()`.
  3. Write to option bytes with `flash_OB_write_data_16()`. This internally erases them and restores other values.
  4. Lock the flash again with `flash_lock()`.

Need to find the right spot in your main flash for your variables? Use `flash_calculate_nonvolatile_addr(n)` to work it out. More details, address calculations and usage instructions in `ch32v003_flash.h`.

## Compile-Time vs. Runtime Addresses

You can pick between compile-time or runtime addresses for your flash memory. For compile-time addresses, set `USE_COMPILE_TIME_ADDRESSES` to 1 in your code.

## Function Cheat Sheet

Here's a quick reference for the main functions:

- `flash_calculate_nonvolatile_addr(uint16_t byte_number)`: Calculate the runtime address.
- `flash_set_latency()`: Set flash controller latency during boot.
- `flash_unlock()`: Unlock the main flash for changes.
- `flash_OB_unlock()`: Unlock the option bytes for tweaks.
- `flash_lock()`: Lock the flash after modifications.
- `flash_erase_64b(uint32_t start_addr)`: Erase a 64-byte page.
- `flash_program_16(uint32_t addr, uint16_t data)`: Program 16 bits of data.
- `flash_program_2x8(uint32_t addr, uint8_t byte1, uint8_t byte0)`: Program two 8-bit values.
- `flash_program_float(uint32_t addr, float value)`: Program a float value.
- `flash_get_16(uint32_t addr)`: Read 16 bits of data.
- `flash_get_8(uint32_t addr)`: Read an 8-bit value.
- `flash_get_float(uint32_t addr)`: Read a float value.
- `flash_OB_write_data_16(uint16_t data)`: Write 16 bits of data to option bytes.
- `flash_OB_write_data_2x8(uint8_t data1, uint8_t data0)`: Write two 8-bit values to option bytes.
- `flash_OB_get_USER()`: Read the USER option byte.
- `flash_OB_get_RDPR()`: Read the RDPR option byte.
- `flash_OB_get_WRPR1()`: Read the WRPR1 option byte.
- `flash_OB_get_WRPR0()`: Read the WRPR0 option byte.
- `flash_OB_get_DATA1()`: Read the DATA1 option byte.
- `flash_OB_get_DATA0()`: Read the DATA0 option byte.
- `flash_OB_get_DATA_16()`: Read both DATA1 and DATA0 option bytes as a 16-bit value.

## Quick Tips

- Erasing data must be done in 64-bit chunks. (ie pages)
- Don't try writing outside the main flash address space; it might turn your microcontroller into a popsicle.
- Fun fact: Option bytes store data as `IIIIIIII DDDDDDDD`, where `D` is data (byte0), and `I` is the inverse of data (byte1).
- To write a byte (8 bits), you need a 16-bit value (`uint16_t`) with the upper 16 bits (`I`) being the inverted pattern of `D`.

## Meet the Creators

- recallmenot (Original code author)
- Tal G


[GitHub Repository](https://github.com/recallmenot/ch32v003fun/tree/8203b1dcddd8271ef04e3959b6375809b9b2df95/examples/flash_storage_main) (Original Code Found Here)

Now, go ahead and have some flash-tastic fun managing your CH32V003 microcontroller's memory!
