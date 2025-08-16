#ifndef _BOARDS_TTX4_BOX_H
#define _BOARDS_TTX4_BOX_H

//------------- BOARD INFO -------------//

// For board detection
#define TTX4_BOARD

//------------- PIN / HARDWARE -------------//

#define TTX4_UART_SLOT_DEBUG uart0
#define PIN_TTX4_UART_TX 0
#define PIN_TTX4_UART_RX 1

#define PIN_TTX4_LED 28
#define PIN_TTX4_WS2812 29

#define PIN_TTX4_DIP1 26
#define PIN_TTX4_DIP2 27

#define PIN_TTX4_BTN1 9
#define PIN_TTX4_BTN2 10
#define PIN_TTX4_BTN3 4
#define PIN_TTX4_BTN4 11

//------------- CRYSTAL -------------//

// On some samples, the xosc can take longer to stabilize than is usual
#ifndef PICO_XOSC_STARTUP_DELAY_MULTIPLIER
#define PICO_XOSC_STARTUP_DELAY_MULTIPLIER 64
#endif

//------------- FLASH -------------//

// slower generic access
// #define PICO_BOOT_STAGE2_CHOOSE_GENERIC_03H 1

// faster winbond commands.
#define PICO_BOOT_STAGE2_CHOOSE_W25Q080 1

// lower is better.
#ifndef PICO_FLASH_SPI_CLKDIV
#define PICO_FLASH_SPI_CLKDIV 2
#endif

// pico_cmake_set_default PICO_FLASH_SIZE_BYTES = (8 * 1024 * 1024)
#ifndef PICO_FLASH_SIZE_BYTES
#define PICO_FLASH_SIZE_BYTES (8 * 1024 * 1024)
#endif

// All boards have B1 RP2040
#ifndef PICO_RP2040_B0_SUPPORTED
#define PICO_RP2040_B0_SUPPORTED 0
#endif

#endif
