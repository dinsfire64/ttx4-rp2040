#include "ws2812.h"
#include "ws2812.pio.h"

#include "hardware/pio.h"
#include "pico/stdlib.h"

#define WS2812_PIO pio1

uint32_t led_data[MAX_NUM_PIXELS];

void init_ws2812()
{
    // TODO: get free sm
    PIO pio = WS2812_PIO;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);

    ws2812_program_init(pio, sm, offset, PIN_TTX4_WS2812, 800000, false);

    // clear LED state.
    for (int i = 0; i < MAX_NUM_PIXELS; i++)
    {
        set_rgb_index(i, 0, 0, 0);
    }

    push_ws1812_state();
}

static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b)
{
    // clamp values of rgb to make it less bright.
    return ((uint32_t)(r * WS2812_MAX_BRIGHTNESS) << 8) |
           ((uint32_t)(g * WS2812_MAX_BRIGHTNESS) << 16) |
           (uint32_t)(b * WS2812_MAX_BRIGHTNESS);
}

void push_ws1812_state()
{
    for (int i = 0; i < MAX_NUM_PIXELS; i++)
    {
        pio_sm_put_blocking(WS2812_PIO, 0, led_data[i] << 8);
    }
}

void set_rgb_index(uint8_t index, uint8_t r, uint8_t g, uint8_t b)
{
    if (index < MAX_NUM_PIXELS)
    {
        led_data[index] = urgb_u32(r, g, b);
    }
}

void set_white_index(uint8_t index, uint8_t brightness)
{
    if (index < MAX_NUM_PIXELS)
    {
        led_data[index] = urgb_u32(brightness, brightness, brightness);
    }
}