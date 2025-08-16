#ifndef _USB_DESC_H_
#define _USB_DESC_H_

#include "ttx4_usb.h"

enum
{
    ITF_TTX4 = 0x00,
    ITF_TTX4_NUM_TOTAL
};

typedef struct TU_ATTR_PACKED
{
    tusb_desc_configuration_t config;

    tusb_desc_interface_t TTX4_interface;

    tusb_desc_endpoint_t TTX4_reportINEndpoint;
    tusb_desc_endpoint_t TTX4_reportOUTEndpoint;
} ttx4_cfg_desc_t;

extern const tusb_desc_device_t TTX4_desc_device;
extern const char *TTX4_string_desc_arr[];
extern const ttx4_cfg_desc_t TTX4_desc_fs_configuration;

// String Descriptor Index
enum
{
    STRID_LANGID = 0,
    STRID_MANUFACTURER,
    STRID_PRODUCT,
};

static char const *string_desc_arr[] =
    {
        (const char[]){0x09, 0x04}, // 0: is supported language is English (0x0409)
        "TAITO CORP.;",             // 1: Manufacturer

        // NOTE: the 0032 matches the BCD value for this device...interesting...
        "USB-IO P.C.B.;0032;", // 2: Product
};

#define MSOS_DESC_BYTE 0xEE
// Note: this is ascii 'a' as matched in the string.
// TODO: make this a string preprocessor.
#define MSOS_VENDOR_BYTE 0x61

static char const *msft100 = "MSFT100a";

// tell windows we want a winusb driver.
static const uint8_t winusb_compat[] = {0x28, 0x00, 0x00, 0x00,
                                        0x00, 0x01,
                                        0x04, 0x00,
                                        0x01,
                                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                        ITF_TTX4,
                                        0x01,
                                        'W', 'I', 'N', 'U', 'S', 'B', '\0', '\0',
                                        //'L', 'I', 'B', 'U', 'S', 'B', '\0', '\0',
                                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

static const uint8_t MS_OS_Descriptor[] =
    {
        // length (136 byte property + 10 byte header = 146 bytes)
        0x92, 0x00, 0x00, 0x00,

        // version 1
        0x00, 0x01,

        // Extended PropertyDescriptor index (5)
        0x05, 0x00,

        // one section
        0x01, 0x00,

        // Size of the property section (4 + 4 + 2 + 42 + 4 + 80 = 136 bytes)
        0x88, 0x00, 0x00, 0x00,

        // Property data type (7 = Unicode REG_MULTI_SZ)
        0x07, 0x00, 0x00, 0x00,

        // Property name length (42 bytes)
        0x2A, 0x00,

        // DeviceInterfaceGUIDs
        'D', 0x00, 'e', 0x00, 'v', 0x00, 'i', 0x00,
        'c', 0x00, 'e', 0x00, 'I', 0x00, 'n', 0x00,
        't', 0x00, 'e', 0x00, 'r', 0x00, 'f', 0x00,
        'a', 0x00, 'c', 0x00, 'e', 0x00, 'G', 0x00,
        'U', 0x00, 'I', 0x00, 'D', 0x00, 's', 0x00, 0x00, 0x00,

        // Property data length (80 bytes = one guids, two nuls, 76 + 2*2)
        0x50, 0x00, 0x00, 0x00,

        // GUID (76 bytes)
        // {46c3136f-0fd5-4973-bc5a-f4a9f3528139}
        // NOTE: the game uses this to open, so it must match.
        '{', 0x00, '4', 0x00, '6', 0x00, 'c', 0x00,
        '3', 0x00, '1', 0x00, '3', 0x00, '6', 0x00,
        'f', 0x00, '-', 0x00, '0', 0x00, 'f', 0x00,
        'd', 0x00, '5', 0x00, '-', 0x00, '4', 0x00,
        '9', 0x00, '7', 0x00, '3', 0x00, '-', 0x00,
        'b', 0x00, 'c', 0x00, '5', 0x00, 'a', 0x00,
        '-', 0x00, 'f', 0x00, '4', 0x00, 'a', 0x00,
        '9', 0x00, 'f', 0x00, '3', 0x00, '5', 0x00,
        '2', 0x00, '8', 0x00, '1', 0x00, '3', 0x00,
        '9', 0x00, '}', 0x00,

        0x00, 0x00, // one null
        0x00, 0x00, // one null

};

#endif