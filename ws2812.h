#ifndef _WS2812_H_
#define _WS2812_H_

#include <stdint.h>

#define MAX_NUM_PIXELS 20

#define WS2812_MAX_BRIGHTNESS (0.10)

void init_ws2812();

void set_rgb_index(uint8_t index, uint8_t r, uint8_t g, uint8_t b);
void set_white_index(uint8_t index, uint8_t brightness);
void push_ws1812_state();

#endif