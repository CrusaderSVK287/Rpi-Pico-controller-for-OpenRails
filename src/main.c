#include "pico/stdlib.h"
#include "pico/time.h"
#include "bsp/board.h"
#include "hardware/adc.h"
#include "tusb.h"
#include "class/hid/hid.h"
#include <string.h>

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

uint8_t const desc_hid_report[] = {
    HID_USAGE_PAGE(HID_USAGE_PAGE_GENERIC_DESKTOP),
    HID_USAGE(HID_USAGE_JOYSTICK),
    HID_COLLECTION(HID_COLLECTION_APPLICATION),
        HID_USAGE(HID_USAGE_X),
        HID_LOGICAL_MIN(0),
        HID_LOGICAL_MAX(1023),
        HID_REPORT_SIZE(8),
        HID_REPORT_COUNT(2),  // 2 bytes for 16-bit analog value
        HID_INPUT(HID_DATA | HID_VARIABLE | HID_ABSOLUTE),
        HID_USAGE_PAGE(HID_USAGE_PAGE_BUTTON),
        HID_USAGE_MIN(1),
        HID_USAGE_MAX(1),
        HID_REPORT_SIZE(8),
        HID_REPORT_COUNT(1),  // 1 byte for button
        HID_INPUT(HID_DATA | HID_VARIABLE | HID_ABSOLUTE),
    HID_COLLECTION_END,
};

uint8_t const* tud_hid_descriptor_report_cb(uint8_t instance) {
    (void) instance;
    return desc_hid_report;
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

uint8_t const* tud_descriptor_device_cb(void) {
    return (uint8_t const*) &desc_device;
}

void send_input_report(uint16_t x_axis, bool button_pressed)
{
    // to avoid compiler complaining for unused parameters
    (void) x_axis;
    (void) button_pressed;

    if (!tud_hid_ready()) return;

    adc_select_input(0);
    uint16_t analog = adc_read(); // set up for test analog pin
    bool digital = gpio_get(5); // test digital pin

    uint8_t report[3];
    report[0] = analog & 0xFF;
    report[1] = (analog >> 8) & 0xFF;
    report[2] = digital ? 0x01 : 0x00; // LSB is button, remaining bits are padding

    tud_hid_report(0, report, sizeof(report));
}

// Required TinyUSB callback – called on setup
void tud_mount_cb(void) {
    // Device mounted
    if (tud_hid_ready()) {
        send_input_report(678, true);       
    }
}

void tud_unmount_cb(void) {
    // Device unmounted
}

void tud_suspend_cb(bool remote_wakeup_en) {
    // USB suspended
}

void tud_resume_cb(void) {
    // USB resumed
}

void tud_hid_report_complete_cb(uint8_t instance, uint8_t const* report, uint16_t len)
{
    send_input_report(942, true);    
}

// Main loop
int main(void)
{
    board_init();
    tusb_init();

    adc_gpio_init(26);  // test analog pin
    gpio_init(5);       // test digital pin
    gpio_set_dir(5, GPIO_IN); // set direction to input

    adc_init();

    while (1)
    {
        tud_task(); // tinyusb device task
    }
}
#define CONFIG_TOTAL_LEN    (TUD_CONFIG_DESC_LEN + TUD_HID_DESC_LEN)

uint8_t const desc_configuration[] = {
    // Configuration Descriptor
    TUD_CONFIG_DESCRIPTOR(1, 1, 0, CONFIG_TOTAL_LEN, 0x00, 100),

    // HID Interface Descriptor
    TUD_HID_DESCRIPTOR(0, 0, HID_ITF_PROTOCOL_NONE, sizeof(desc_hid_report), 0x81, 64, 10)
};

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
    "Lukas Devices",               // 1: Manufacturer
    "Pico HID Controller",         // 2: Product
    "123456",                      // 3: Serial number
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
