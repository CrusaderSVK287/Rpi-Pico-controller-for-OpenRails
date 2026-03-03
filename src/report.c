#include "report.h"

/*
 * #############################################
 * # This file contains code for HID reports   #
 * # and device descriptor, mostly boilerplate #
 * # code that simply has to be defined        #
 * #############################################
 */

// HID Usage Pages
#define HID_USAGE_PAGE_GENERIC_DESKTOP    0x01
#define HID_USAGE_PAGE_BUTTON             0x09

// Generic Desktop Page Usages
#define HID_USAGE_JOYSTICK                0x04
#define HID_USAGE_X                       0x30

// HID Collection types
#define HID_COLLECTION_APPLICATION        0x01

// HID Input flags
#define HID_DATA                         0x00
#define HID_CONSTANT                     0x01
#define HID_ARRAY                        0x00
#define HID_VARIABLE                     0x02
#define HID_ABSOLUTE                     0x00
#define HID_RELATIVE                     0x04

#define CONFIG_TOTAL_LEN    (TUD_CONFIG_DESC_LEN + TUD_HID_DESC_LEN)

uint8_t const desc_hid_report[] = {
    HID_USAGE_PAGE(HID_USAGE_PAGE_GENERIC_DESKTOP),
    HID_USAGE(HID_USAGE_JOYSTICK),
    HID_COLLECTION(HID_COLLECTION_APPLICATION),

        // Analog axes
        HID_USAGE(HID_USAGE_X),
        HID_LOGICAL_MIN(0),
        HID_LOGICAL_MAX(1023),
        HID_REPORT_SIZE(8),          // 1 byte
        HID_REPORT_COUNT(6),         // 2 bytes per axis × 3 axes = 6 bytes
        HID_INPUT(HID_DATA | HID_VARIABLE | HID_ABSOLUTE),

        // Digital inputs: 1 button + 5 two-state switches
        HID_USAGE_PAGE(HID_USAGE_PAGE_BUTTON),
        HID_USAGE_MIN(1),
        HID_USAGE_MAX(6),            // 1 button + 5 switches
        HID_REPORT_SIZE(1),          // 1 bit per button/switch
        HID_REPORT_COUNT(6),
        HID_INPUT(HID_DATA | HID_VARIABLE | HID_ABSOLUTE),

        // Padding to complete the byte
        HID_REPORT_SIZE(2),
        HID_REPORT_COUNT(1),
        HID_INPUT(HID_CONSTANT),

        // Multi-state switches
        // 3-state switch (2 bits)
        HID_USAGE_MIN(7),
        HID_USAGE_MAX(7),
        HID_LOGICAL_MIN(0),
        HID_LOGICAL_MAX(2),
        HID_REPORT_SIZE(2),
        HID_REPORT_COUNT(1),
        HID_INPUT(HID_DATA | HID_VARIABLE | HID_ABSOLUTE),

        // 4-state switch (2 bits)
        HID_USAGE_MIN(8),
        HID_USAGE_MAX(8),
        HID_LOGICAL_MIN(0),
        HID_LOGICAL_MAX(3),
        HID_REPORT_SIZE(2),
        HID_REPORT_COUNT(1),
        HID_INPUT(HID_DATA | HID_VARIABLE | HID_ABSOLUTE),

        // Padding to complete the second byte
        HID_REPORT_SIZE(4),
        HID_REPORT_COUNT(1),
        HID_INPUT(HID_CONSTANT),

    HID_COLLECTION_END
};

uint8_t const* tud_hid_descriptor_report_cb(uint8_t instance) {
    (void) instance;
    return desc_hid_report;
}

// Called when host requests a report
uint16_t tud_hid_get_report_cb(uint8_t itf, uint8_t report_id,
                               hid_report_type_t report_type,
                               uint8_t* buffer, uint16_t reqlen) {
    (void) itf;
    (void) report_id;
    (void) report_type;
    (void) buffer;
    (void) reqlen;
    return 0;
}

// Called when host sends a SET_REPORT control transfer
void tud_hid_set_report_cb(uint8_t itf, uint8_t report_id,
                           hid_report_type_t report_type,
                           uint8_t const* buffer, uint16_t bufsize) {
    (void) itf;
    (void) report_id;
    (void) report_type;
    (void) buffer;
    (void) bufsize;
}

// Minimal device descriptor
tusb_desc_device_t const desc_device = {
    .bLength            = sizeof(tusb_desc_device_t),
    .bDescriptorType    = TUSB_DESC_DEVICE,
    .bcdUSB             = 0x0200,
    .bDeviceClass       = 0x00,
    .bDeviceSubClass    = 0x00,
    .bDeviceProtocol    = 0x00,
    .bMaxPacketSize0    = CFG_TUD_ENDPOINT0_SIZE,

    .idVendor           = 0xCafe,
    .idProduct          = 0x4000,
    .bcdDevice          = 0x0100,

    .iManufacturer      = 0x01,
    .iProduct           = 0x02,
    .iSerialNumber      = 0x03,

    .bNumConfigurations = 1
};

uint8_t const desc_configuration[] = {
    // Configuration Descriptor
    TUD_CONFIG_DESCRIPTOR(1, 1, 0, CONFIG_TOTAL_LEN, 0x00, 100),

    // HID Interface Descriptor
    TUD_HID_DESCRIPTOR(0, 0, HID_ITF_PROTOCOL_NONE, sizeof(desc_hid_report), 0x81, 64, 10)
};

uint8_t const* tud_descriptor_device_cb(void) {
    return (uint8_t const*) &desc_device;
}

uint8_t const* tud_descriptor_configuration_cb(uint8_t index) {
    (void) index; // only one config
    return desc_configuration;
}

static uint16_t _desc_str[32];

// English language ID
#define LANGID_EN_US 0x0409

// String descriptors
const char* string_desc_arr[] = {
    (const char[]){ 0x09, 0x04 },   // 0: Supported language (English)
    DESC_MANUFACTURER,
    DESC_PRODUCT,
    DESC_SERIALNUMBER,
};

// Buffer to hold UTF-16 descriptor
static uint16_t _desc_str[32];

uint16_t const* tud_descriptor_string_cb(uint8_t index, uint16_t langid)
{
    uint8_t chr_count;

    if (index == 0)
    {
        // Return supported language
        _desc_str[1] = LANGID_EN_US;
        chr_count = 1;
    }
    else
    {
        if (index >= sizeof(string_desc_arr) / sizeof(string_desc_arr[0])) return NULL;

        const char* str = string_desc_arr[index];

        // Convert ASCII to UTF-16
        chr_count = strlen(str);
        if (chr_count > 31) chr_count = 31;

        for (uint8_t i = 0; i < chr_count; i++)
        {
            _desc_str[1 + i] = str[i];
        }
    }

    // First byte: length (including header), second byte: descriptor type
    _desc_str[0] = (TUSB_DESC_STRING << 8) | (2 * chr_count + 2);

    return _desc_str;
}

