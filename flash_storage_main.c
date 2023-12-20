/**
 * @file main.c
 * @brief This is the main source file for the flash read/write example.
 * @details 
 * This includes flash memory operations to store 
 * small amounts of infrequently-changing user 
 * information. (ie settings, etc)
 * 
 * Integrated flash has limited write cycles (10K as 
 * per datasheet) and will thus wear out quickly 
 * if not used carefully
 * 
 * Authors: Tal G and based on recallmenot 
 * (on Github)'s example code that was removed 
 * from the main ch32v003fun examples repository.
 * 
 * Reference here:
 * https://github.com/recallmenot/ch32v003fun/tree/8203b1dcddd8271ef04e3959b6375809b9b2df95/examples/flash_storage_main
 * 
 */
#include <stdint.h>
#define SYSTEM_CORE_CLOCK 48000000
#define APB_CLOCK SYSTEM_CORE_CLOCK

// Include the ch32v003fun header file for CH32V003 hardware
#include "../ch32v003fun/ch32v003fun/ch32v003fun.h" 

#include <stdio.h>
#include "ch32v003_flash.h"

// Define this constant to use compile-time 
// flash addresses in the below example code.
#define USE_COMPILE_TIME_ADDRESSES 1

#ifdef USE_COMPILE_TIME_ADDRESSES
	// Compile-time determined address for flash memory
	#define NONVOLATILE_START_ADDR FLASH_PRECALCULATE_NONVOLATILE_ADDR(0)
	#define NONVLOATILE_VAR_ADDR FLASH_PRECALCULATE_NONVOLATILE_ADDR(10)
#else
	// Runtime determined address for flash memory
	uint32_t nonvolatile_var_addr;
	uint32_t nonvolatile_start_addr;
#endif

uint16_t valueInFlash;

/**
 * @brief The main function.
 * @details Initializes hardware, performs flash memory operations, and blinks an LED on each loop.
 * @return 0 upon successful execution.
 */
int main()
{
	SystemInit(); // Initialize the system hardware.

	printf("Starting..");
	Delay_Ms(3000); // Delay for 3000 milliseconds (3 seconds).

	printf("\r\non-volatile storage testing\r\n");

	// Enable GPIOs for LED blinking on pin C4
	RCC->APB2PCENR |=  RCC_APB2Periph_GPIOC;
	
	// Configure GPIO C4 as Push-Pull with a speed of 50MHz
	GPIOC->CFGLR &= ~(0xf<<(4*0));
	GPIOC->CFGLR |= (GPIO_Speed_50MHz | GPIO_CNF_OUT_PP)<<(4*0);

	// Set the flash latency for proper operation.
	flash_set_latency(); 

	printf("FLASH_LENGTH_OVERRIDE is        %u\r\n", (uint16_t)(uintptr_t)FLASH_LENGTH_OVERRIDE);

	#ifdef USE_COMPILE_TIME_ADDRESSES
		printf("non-volatile start address is   %lu\r\n", NONVOLATILE_START_ADDR);
		printf("non-volatile var address is     %lu\r\n", NONVLOATILE_VAR_ADDR);
	#else
		nonvolatile_start_addr = flash_calcualte_nonvolatile_addr(0); // Calculate the start address for non-volatile data at runtime.
		nonvolatile_var_addr = flash_calcualte_nonvolatile_addr(10);  // Calculate the variable address for non-volatile data at runtime.
		printf("non-volatile start address is   %lu\r\n", nonvolatile_start_addr);
		printf("non-volatile var address is     %lu\r\n", nonvolatile_var_addr);
	#endif

	int writeCount = 0;

	while(1)
	{

		GPIOC->BSHR = (1<<(16+0)); // Set pin C4 low (LED on).

		#ifdef USE_COMPILE_TIME_ADDRESSES
			valueInFlash = flash_read_16_bits(NONVLOATILE_VAR_ADDR); // Read a 16-bit value from flash memory at the predefined address.
		#else
			valueInFlash = flash_read_16_bits(nonvolatile_var_addr); // Read a 16-bit value from flash memory at the runtime determined address.
		#endif
		
		printf("   Saved value is %u\r\n", valueInFlash);

		Delay_Ms(250); // Delay for 250 milliseconds.
		GPIOC->BSHR = (1<<0); // Set pin C4 high (LED off).
		Delay_Ms(250); // Delay for 250 milliseconds.

		// Prevent more than 5 writes per power-on, as writing to flash is a limited resource.
		if(writeCount >= 5) { 
			printf("Done writing. Wait...\r\n");
			Delay_Ms(1000); // Delay for 1000 milliseconds (1 second).
			continue;
		}
		writeCount++;

		// Decrease the value to be written to flash by 1
		valueInFlash--;

		// Unlock writing to flash memory
		flash_unlock();
		printf("Memory unlocked\r\n");

		// This erases a 64-byte page in the flash memory.
		// Note: smaller erases are not possible on the CH32V003.
		#ifdef USE_COMPILE_TIME_ADDRESSES
			flash_erase_page(NONVOLATILE_START_ADDR); // Erase a 64-bit block of flash memory at the predefined start address.
		#else
			flash_erase_page(nonvolatile_start_addr); // Erase a 64-bit block of flash memory at the runtime determined start address.
		#endif
		printf("Memory erased\r\n");

		#ifdef USE_COMPILE_TIME_ADDRESSES
			flash_program_16(NONVLOATILE_VAR_ADDR, valueInFlash); // Write a 16-bit value to flash memory at the predefined address.
		#else
			flash_program_16(nonvolatile_var_addr, valueInFlash); // Write a 16-bit value to flash memory at the runtime determined address.
		#endif
		printf("Memory written\r\n");

		// Lock flash memory
		flash_lock();
		printf("Memory locked. \r\nWaiting...\r\n");
		
		Delay_Ms(15000); // Delay for 15000 milliseconds (~15 seconds).
	}
}
