#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "tusb.h"
#include "debug.h"

#include "ttx4_usb.h"
#include "ttx4_structs.h"
#include "ws2812.h"

uint8_t temp_buff_cpu[TTX4_CMDEPSIZE] = {0};
uint8_t temp_buff_state[TTX4_CMDEPSIZE] = {0};

ttx4_togame_t to_game_buff;
ttx4_fromgame_t from_game_buff;

// magic values, constant.
const unsigned char nesicaStart[] = {0x04, 0xE3, 0xDA, 0xC2, 0x8C, 0x56, 0x80};

// fake id for testing.
unsigned char taitoidFromServer[] = {
    'a',
    'a',
    'a',
    'a',

    'b',
    'b',
    'b',
    'b',

    'c',
    'c',
    'c',
    'c',

    'd',
    'd',
    'd',
    'd',
};

void setup_pin(uint32_t pin)
{
    gpio_init(pin);
    gpio_set_dir(pin, GPIO_IN);
    gpio_pull_up(pin);
}

int main(void)
{
    DebugSetup();

    setup_pin(PIN_TTX4_BTN1);
    setup_pin(PIN_TTX4_BTN2);
    setup_pin(PIN_TTX4_BTN3);
    setup_pin(PIN_TTX4_BTN4);

    setup_pin(PIN_TTX4_DIP1);
    setup_pin(PIN_TTX4_DIP2);

    init_ws2812();

    // init device stack on configured roothub port
    tud_init(BOARD_TUD_RHPORT);

    while (1)
    {
        // tinyusb device task
        tud_task();

        if (tud_ready())
        {
            if (!usbd_edpt_busy(0, TTX4_CMD_FROMGAME_EPADDR))
            {
                usbd_edpt_xfer(0, TTX4_CMD_FROMGAME_EPADDR, temp_buff_cpu, TTX4_CMDEPSIZE);

                // process data if changed.
                if (memcmp(temp_buff_cpu, from_game_buff.raw, TTX4_CMDEPSIZE))
                {
                    memcpy(from_game_buff.raw, temp_buff_cpu, TTX4_CMDEPSIZE);

                    DebugOutputBuffer("CPU", temp_buff_cpu, TTX4_CMDEPSIZE);

                    // push the lights from the game to a ws2812 strip.
                    set_rgb_index(0,
                                  from_game_buff.light_stick_top_r,
                                  from_game_buff.light_stick_top_g,
                                  from_game_buff.light_stick_top_b);

                    set_rgb_index(1,
                                  from_game_buff.light_stick_middle_r,
                                  from_game_buff.light_stick_middle_g,
                                  from_game_buff.light_stick_middle_b);

                    set_rgb_index(2,
                                  from_game_buff.light_stick_bottom_r,
                                  from_game_buff.light_stick_bottom_g,
                                  from_game_buff.light_stick_bottom_b);

                    set_rgb_index(3,
                                  from_game_buff.nesica_reader_r,
                                  from_game_buff.nesica_reader_g,
                                  from_game_buff.nesica_reader_b);

                    set_white_index(4, from_game_buff.light1);
                    set_white_index(5, from_game_buff.light2);
                    set_white_index(6, from_game_buff.light3);
                    set_white_index(7, from_game_buff.light4);
                    set_white_index(8, from_game_buff.light5);
                    set_white_index(9, from_game_buff.light6);
                    set_white_index(10, from_game_buff.light7);
                    set_white_index(11, from_game_buff.light8);
                    set_white_index(12, from_game_buff.light9);
                    set_white_index(13, from_game_buff.light_confirm);

                    push_ws1812_state();
                }
            }

            if (!usbd_edpt_busy(0, TTX4_CMD_TOGAME_EPADDR))
            {
                // create report from scratch
                memset(to_game_buff.raw, 0, TTX4_CMDEPSIZE);

                to_game_buff.always_x20_0 = 0x20;
                to_game_buff.always_x20_1 = 0x20;

                to_game_buff.test_sw = !gpio_get(PIN_TTX4_DIP1);
                to_game_buff.service_sw = !gpio_get(PIN_TTX4_DIP2);

                to_game_buff.decision_button = !gpio_get(PIN_TTX4_BTN1);
                to_game_buff.select_sw = !gpio_get(PIN_TTX4_BTN2);
                to_game_buff.coin_sw = !gpio_get(PIN_TTX4_BTN3);

                // buttons are active high, so map them here...
                // to_game_buff.rhythm_button_1 = !gpio_get();
                // etc...

                // check to see if the game is asking for a card.
                if (from_game_buff.nesica_state == 0xC0 || from_game_buff.nesica_state == 0xFF)
                {
                    // if we have a card to insert...
                    // for now this is just a test button.
                    if (!gpio_get(PIN_TTX4_BTN4))
                    {
                        // confirm card insertion
                        to_game_buff.nesica_state = 0x19;

                        // copy the read id to the report.
                        memcpy(to_game_buff.card_id, taitoidFromServer, sizeof(to_game_buff.card_id));
                    }
                }

                // copy the magic bytes always.
                memcpy(to_game_buff.nesica_magic, nesicaStart, sizeof(to_game_buff.nesica_magic));

                usbd_edpt_xfer(0, TTX4_CMD_TOGAME_EPADDR, to_game_buff.raw, TTX4_CMDEPSIZE);

#if ENABLE_BUFFER_DUMP
                // for debugging, print the output on changes.
                if (memcmp(temp_buff_state, to_game_buff.raw, sizeof(to_game_buff.raw)))
                {
                    DebugOutputBuffer("STA", temp_buff_state, TTX4_CMDEPSIZE);
                    memcpy(temp_buff_state, to_game_buff.raw, sizeof(to_game_buff.raw));
                }
#endif
            }
        }
    }
}

//--------------------------------------------------------------------+
// Device callbacks
//--------------------------------------------------------------------+

// Invoked when device is mounted
void tud_mount_cb(void)
{
}

// Invoked when device is unmounted
void tud_umount_cb(void)
{
}

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en)
{
    (void)remote_wakeup_en;
}

// Invoked when usb bus is resumed
void tud_resume_cb(void)
{
}
