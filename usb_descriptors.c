
#include "tusb.h"
#include "usb_descriptors.h"
#include "ttx4_usb.h"
#include "debug.h"

const ttx4_cfg_desc_t TTX4_desc_fs_configuration = {
    .config = {
        .bLength = sizeof(tusb_desc_configuration_t),
        .bDescriptorType = TUSB_DESC_CONFIGURATION,

        .wTotalLength = sizeof(ttx4_cfg_desc_t),
        .bNumInterfaces = ITF_TTX4_NUM_TOTAL,
        .bConfigurationValue = 1,
        .iConfiguration = 0,
        .bmAttributes = 0xc0,
        .bMaxPower = 0,
    },
    .TTX4_interface = {
        .bLength = sizeof(tusb_desc_interface_t),
        .bDescriptorType = TUSB_DESC_INTERFACE,

        .bInterfaceNumber = ITF_TTX4,
        .bAlternateSetting = 0,
        .bNumEndpoints = 2,
        .bInterfaceClass = 255,
        .bInterfaceSubClass = 0,
        .bInterfaceProtocol = 0,
        .iInterface = 0,
    },
    .TTX4_reportINEndpoint = {
        .bLength = sizeof(tusb_desc_endpoint_t),
        .bDescriptorType = TUSB_DESC_ENDPOINT,

        .bEndpointAddress = TTX4_CMD_TOGAME_EPADDR,
        .bmAttributes = {
            .xfer = TUSB_XFER_BULK,
            .sync = TUSB_ISO_EP_ATT_NO_SYNC,
            .usage = 0x00,
        },
        .wMaxPacketSize = TTX4_CMDEPSIZE,
        .bInterval = 0,
    },
    .TTX4_reportOUTEndpoint = {
        .bLength = sizeof(tusb_desc_endpoint_t),
        .bDescriptorType = TUSB_DESC_ENDPOINT,

        .bEndpointAddress = TTX4_CMD_FROMGAME_EPADDR,
        .bmAttributes = {
            .xfer = TUSB_XFER_BULK,
            .sync = TUSB_ISO_EP_ATT_NO_SYNC,
            .usage = 0x00,
        },
        .wMaxPacketSize = TTX4_CMDEPSIZE,
        .bInterval = 0,
    },
};

//--------------------------------------------------------------------+
// Device Descriptors
//--------------------------------------------------------------------+
tusb_desc_device_t const desc_device =
    {
        .bLength = sizeof(tusb_desc_device_t),
        .bDescriptorType = TUSB_DESC_DEVICE,
        .bcdUSB = 0x0200,
        .bDeviceClass = 0x00,
        .bDeviceSubClass = 0x00,
        .bDeviceProtocol = 0x00,
        .bMaxPacketSize0 = CFG_TUD_ENDPOINT0_SIZE,

        .idVendor = TTX4_VID,
        .idProduct = TTX4_PID,
        .bcdDevice = TTX4_BCD,

        .iManufacturer = 0x01,
        .iProduct = 0x02,
        .iSerialNumber = 0,

        .bNumConfigurations = 0x01,
};

// Invoked when received GET DEVICE DESCRIPTOR
// Application return pointer to descriptor
uint8_t const *tud_descriptor_device_cb(void)
{
    return (uint8_t const *)&desc_device;
}

//--------------------------------------------------------------------+
// Configuration Descriptor
//--------------------------------------------------------------------+

// Invoked when received GET CONFIGURATION DESCRIPTOR
// Application return pointer to descriptor
// Descriptor contents must exist long enough for transfer to complete
uint8_t const *tud_descriptor_configuration_cb(uint8_t index)
{
    (void)index; // for multiple configurations
    return (uint8_t const *)&TTX4_desc_fs_configuration;
}

//--------------------------------------------------------------------+
// String Descriptors
//--------------------------------------------------------------------+

static uint16_t _desc_str[32 + 1];

// Invoked when received GET STRING DESCRIPTOR request
// Application return pointer to descriptor, whose contents must exist long enough for transfer to complete
uint16_t const *tud_descriptor_string_cb(uint8_t index, uint16_t langid)
{
    (void)langid;
    size_t chr_count;

    switch (index)
    {
    case STRID_LANGID:
        memcpy(&_desc_str[1], string_desc_arr[0], 2);
        chr_count = 1;
        break;

    case MSOS_DESC_BYTE:
        chr_count = strlen(msft100);

        // Convert ASCII string into UTF-16
        for (size_t i = 0; i < chr_count; i++)
        {
            _desc_str[1 + i] = msft100[i];
        }
        break;

    default:
        if (!(index < sizeof(string_desc_arr) / sizeof(string_desc_arr[0])))
            return NULL;

        const char *str = string_desc_arr[index];

        // Cap at max char
        chr_count = strlen(str);
        size_t const max_count = sizeof(_desc_str) / sizeof(_desc_str[0]) - 1; // -1 for string type
        if (chr_count > max_count)
            chr_count = max_count;

        // Convert ASCII string into UTF-16
        for (size_t i = 0; i < chr_count; i++)
        {
            _desc_str[1 + i] = str[i];
        }
        break;
    }

    // first byte is length (including header), second byte is string type
    _desc_str[0] = (uint16_t)((TUSB_DESC_STRING << 8) | (2 * chr_count + 2));

    return _desc_str;
}

//function used to implement the Microsoft WinUSB GUID bs to assign the correct driver.
bool custom_command_cb(uint8_t rhport, uint8_t stage, tusb_control_request_t const *request)
{
    bool result = false;

    DebugPrintf("custom_command_cb %02x", request->wIndex);

    if (rhport != BOARD_TUD_RHPORT)
        return result;

    if (request->bmRequestType_bit.direction == TUSB_DIR_IN)
    {
        // TUSB_DIR_IN
        switch (stage)
        {
        case CONTROL_STAGE_SETUP:

            switch (request->wIndex)
            {
            case 4:
                result = tud_control_xfer(rhport, request, &winusb_compat, sizeof(winusb_compat));
                break;

            case 5:
                result = tud_control_xfer(rhport, request, &MS_OS_Descriptor, sizeof(MS_OS_Descriptor));
                break;

            default:
                DebugPrintf("UNKN custom_command_cb %04x", request->wIndex);
                break;
            }
            break;
        case CONTROL_STAGE_DATA:
            result = true;
            break;
        case CONTROL_STAGE_ACK:
            result = true;
            break;
        default:
            break;
        }
    }
    else
    {
        // TUSB_DIR_OUT
        switch (stage)
        {
        case CONTROL_STAGE_SETUP:
            // result = tud_control_xfer(rhport, request, &to_send, sizeof(to_send));
            break;
        case CONTROL_STAGE_DATA:
            result = true;
            break;
        case CONTROL_STAGE_ACK:
            result = true;
            break;
        default:
            break;
        }
    }
    return result;
}

bool tud_vendor_control_xfer_cb(uint8_t rhport, uint8_t stage, tusb_control_request_t const *request)
{
    DebugPrintf("tud_vendor_control_xfer_cb %02x", request->bRequest);

    bool result = false;
    switch (request->bmRequestType_bit.type)
    {
    case TUSB_REQ_TYPE_VENDOR:
        switch (request->bRequest)
        {
        case MSOS_VENDOR_BYTE:
            result = custom_command_cb(rhport, stage, request);
            break;

        default:
            // do nothing
            break;
        }
        break;
    case TUSB_REQ_TYPE_STANDARD:
        break;
    case TUSB_REQ_TYPE_CLASS:
        break;
    default:
        break;
    }
    return result;
}

// Implement callback to add our custom driver
usbd_class_driver_t const *usbd_app_driver_get_cb(uint8_t *driver_count)
{
    *driver_count = 1;

    return &_ttx4d_driver;
}