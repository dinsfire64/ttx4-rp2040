#include "ttx4_usb.h"
#include "tusb.h"
#include "debug.h"

static uint8_t itf_num;

static void ttx4d_init(void)
{
    DebugPrintf("ttx4d_init");
}

static void ttx4d_reset(uint8_t __unused rhport)
{
    DebugPrintf("ttx4d_reset");
    itf_num = 0;
}

static uint16_t ttx4d_open(uint8_t __unused rhport, tusb_desc_interface_t const *itf_desc, uint16_t max_len)
{

    /*
    TU_VERIFY(TUSB_CLASS_VENDOR_SPECIFIC == itf_desc->bInterfaceClass &&
                  RESET_INTERFACE_SUBCLASS == itf_desc->bInterfaceSubClass &&
                  RESET_INTERFACE_PROTOCOL == itf_desc->bInterfaceProtocol,
              0);
              */

    uint16_t const drv_len = sizeof(tusb_desc_interface_t) +
                             itf_desc->bNumEndpoints * sizeof(tusb_desc_endpoint_t);
    TU_VERIFY(max_len >= drv_len, 0);

    itf_num = itf_desc->bInterfaceNumber;

    uint8_t const *p_desc = tu_desc_next(itf_desc);
    uint8_t found_endpoints = 0;
    while ((found_endpoints < itf_desc->bNumEndpoints) && (drv_len <= max_len))
    {
        tusb_desc_endpoint_t const *desc_ep = (tusb_desc_endpoint_t const *)p_desc;
        if (TUSB_DESC_ENDPOINT == tu_desc_type(desc_ep))
        {
            TU_ASSERT(usbd_edpt_open(rhport, desc_ep));
            found_endpoints += 1;
        }
        p_desc = tu_desc_next(p_desc);
    }

    usbd_sof_enable(rhport, true);

    DebugPrintf("ttx4d_open %d success %d", drv_len, found_endpoints);
    return drv_len;
}
static bool ttx4d_control_request_cb(uint8_t rhport, uint8_t stage, tusb_control_request_t const *request)
{
    DebugPrintf("ttx4d_control_request_cb %02x", request->bRequest);

    if (request->wIndex == itf_num)
    {
        // process data.
    }

    return true;
}

static bool ttx4d_xfer_cb(uint8_t rhport, uint8_t ep_addr, xfer_result_t result, uint32_t xferred_bytes)
{
    // DebugPrintf("ttx4d_xfer_cb %02x", ep_addr);

    return true;
}

static void ttx4d_sof_isr(uint8_t rhport, uint32_t frame_count)
{
    // sof_event_running = true;
}

usbd_class_driver_t _ttx4d_driver =
    {
#if CFG_TUSB_DEBUG >= 2
        .name = "ttx4",
#endif
        .init = ttx4d_init,
        .reset = ttx4d_reset,
        .open = ttx4d_open,
        .control_xfer_cb = ttx4d_control_request_cb,
        .xfer_cb = ttx4d_xfer_cb,
        .sof = ttx4d_sof_isr,
};