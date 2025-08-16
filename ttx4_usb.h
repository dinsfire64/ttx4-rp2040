#ifndef _TTX4_USB_H_
#define _TTX4_USB_H_

#include "device/usbd_pvt.h"

#define TTX4_VID 0x0ae4
#define TTX4_PID 0x0901

// NOTE: this is also replicated in the product string for this device...interesting
#define TTX4_BCD 0x0032

#define ENDPOINT_DIR_OUT 0x00
#define ENDPOINT_DIR_IN 0x80

#define TTX4_CMD_TOGAME_EPADDR (ENDPOINT_DIR_IN | 1)
#define TTX4_CMD_FROMGAME_EPADDR (ENDPOINT_DIR_OUT | 2)

#define TTX4_CMDEPSIZE 64

extern usbd_class_driver_t _ttx4d_driver;

#endif