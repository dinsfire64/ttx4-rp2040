#ifndef _DEBUG_H_
#define _DEBUG_H_

#include "pico/stdlib.h"
#include <stdio.h>
#include <stdarg.h>

#define ENABLE_DEBUG_UART false
#define ENABLE_BUFFER_DUMP false

#define DEBUG_UART_SLOT TTX4_UART_SLOT_DEBUG
#define DEBUG_UART_BAUD 921600
#define DEBUG_UART_OUT_BUFF_SIZE 512

#define DEBUG_UART_TX_PIN PIN_TTX4_UART_TX
#define DEBUG_UART_RX_PIN PIN_TTX4_UART_RX

#define END_LINE "\r\n"

extern bool debug_setup;

void DebugSetup(void);
void DebugPrintf(const char *fmt, ...);
void DebugOutputBuffer(const char *prefix, uint8_t buff[], int len);

#if !(ENABLE_DEBUG_UART)
#define DebugPrintf(char, ...) ;
#endif

#if !(ENABLE_BUFFER_DUMP)
#define DebugOutputBuffer(char, uint8_t, int) ;
#endif

#endif
