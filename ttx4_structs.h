#ifndef _TTX4_LOGIC_H_
#define _TTX4_LOGIC_H_

#include <stdint.h>

#include "ttx4_usb.h"

typedef union
{
    struct
    {
        // always 0x80
        uint8_t always_x80;
        // always 0x01
        uint8_t always_x01;

        uint8_t byte_02;
        uint8_t byte_03;
        uint8_t byte_04;
        // toggles between 0x40 and 0x00 for the lockout coil state.
        uint8_t coil_state;
        uint8_t byte_06;
        uint8_t byte_07;
        uint8_t byte_08;
        uint8_t byte_09;
        uint8_t byte_10;
        uint8_t byte_11;
        uint8_t byte_12;
        uint8_t byte_13;
        uint8_t byte_14;
        uint8_t byte_15;

        uint8_t light_stick_top_r;
        uint8_t light_stick_top_g;
        uint8_t light_stick_top_b;

        uint8_t light_stick_middle_r;
        uint8_t light_stick_middle_g;
        uint8_t light_stick_middle_b;

        uint8_t light_stick_bottom_r;
        uint8_t light_stick_bottom_g;
        uint8_t light_stick_bottom_b;

        uint8_t nesica_reader_r;
        uint8_t nesica_reader_g;
        uint8_t nesica_reader_b;

        // light1 2
        uint8_t light1;
        uint8_t light2;

        // are these lights? they are always zero...
        uint8_t byte_30;
        uint8_t byte_31;

        // lights 3-9, then confirm
        uint8_t light3;
        uint8_t light4;
        uint8_t light5;
        uint8_t light6;
        uint8_t light7;
        uint8_t light8;
        uint8_t light9;
        uint8_t light_confirm;

        // nesica state?
        // toggles between 0xc0 and 0x80
        uint8_t nesica_state;
        uint8_t byte_41;
        uint8_t byte_42;
        uint8_t byte_43;
        uint8_t byte_44;
        uint8_t byte_45;
        uint8_t byte_46;
        uint8_t byte_47;
        uint8_t byte_48;
        uint8_t byte_49;
        uint8_t byte_50;
        uint8_t byte_51;
        uint8_t byte_52;
        uint8_t byte_53;
        uint8_t byte_54;
        uint8_t byte_55;
        uint8_t byte_56;
        uint8_t byte_57;
        uint8_t byte_58;
        uint8_t byte_59;
        uint8_t byte_60;
        uint8_t byte_61;
        uint8_t byte_62;
        uint8_t byte_63;
    } __attribute__((packed));
    uint8_t raw[TTX4_CMDEPSIZE];
} ttx4_fromgame_t;

typedef union
{
    struct
    {
        // byte 0 always 0x20
        uint8_t always_x20_0;
        // byte 1 always 0x20
        uint8_t always_x20_1;
        
        uint8_t byte_02;
        uint8_t byte_03;

        // byte 04
        bool rhythm_button_8 : 1;
        bool rhythm_button_9 : 1;
        bool cansel_button : 1; // intentially misspelt as the service manual uses this spelling, i found it funny.
        bool decision_button : 1;
        bool select_sw : 1;
        bool test_sw : 1;
        bool service_sw : 1;
        bool coin_sw : 1;

        uint8_t byte_05;

        // byte 06
        bool rhythm_button_1 : 1;
        bool rhythm_button_2 : 1;
        bool rhythm_button_3 : 1;
        bool rhythm_button_4 : 1;
        bool rhythm_button_5 : 1;
        bool rhythm_button_6 : 1;
        bool rhythm_button_7 : 1;
        bool enter_sw : 1;

        uint8_t byte_07;
        uint8_t byte_08;
        uint8_t byte_09;
        uint8_t byte_10;
        uint8_t byte_11;
        uint8_t byte_12;
        uint8_t byte_13;
        uint8_t byte_14;
        uint8_t byte_15;
        uint8_t byte_16;
        uint8_t byte_17;
        uint8_t byte_18;
        uint8_t byte_19;
        uint8_t byte_20;
        uint8_t byte_21;
        uint8_t byte_22;
        uint8_t byte_23;
        uint8_t byte_24;
        uint8_t byte_25;
        uint8_t byte_26;
        uint8_t byte_27;
        uint8_t byte_28;
        uint8_t byte_29;
        uint8_t byte_30;
        uint8_t byte_31;
        uint8_t byte_32;
        uint8_t byte_33;
        uint8_t byte_34;
        uint8_t byte_35;
        uint8_t byte_36;
        uint8_t byte_37;
        uint8_t byte_38;
        uint8_t byte_39;
        
        uint8_t nesica_state;

        // nesica magic 7 bytes are [41..47]
        uint8_t nesica_magic[7];

        // card id is the top 16 bytes [48..64]
        uint8_t card_id[16];
    } __attribute__((packed));
    uint8_t raw[TTX4_CMDEPSIZE];
} ttx4_togame_t;

#endif