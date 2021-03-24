/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include "tusb.h"
#include "usb_descriptors.h"

/* A combination of interfaces must have a unique product id, since PC will save device driver after the first plug.
 * Same VID/PID with different interface e.g MSC (first), then CDC (later) will possibly cause system error on PC.
 *
 * Auto ProductID layout's Bitmap:
 *   [MSB]         HID | MSC | CDC          [LSB]
 */
#define _PID_MAP(itf, n)  ( (CFG_TUD_##itf) << (n) )
#define USB_PID           (0x4000 | _PID_MAP(CDC, 0) | _PID_MAP(MSC, 1) | _PID_MAP(HID, 2) | \
                           _PID_MAP(MIDI, 3) | _PID_MAP(VENDOR, 4) )

//--------------------------------------------------------------------+
// Device Descriptors
//--------------------------------------------------------------------+
tusb_desc_device_t const desc_device =
        {
                .bLength            = sizeof(tusb_desc_device_t),
                .bDescriptorType    = TUSB_DESC_DEVICE,
                .bcdUSB             = 0x0200,
                .bDeviceClass       = 0x00,
                .bDeviceSubClass    = 0x00,
                .bDeviceProtocol    = 0x00,
                .bMaxPacketSize0    = CFG_TUD_ENDPOINT0_SIZE,

                .idVendor           = 0x27bb,
                .idProduct          = 0x3bcd,
                .bcdDevice          = 0x0100,

                .iManufacturer      = 0x01,
                .iProduct           = 0x02,
                .iSerialNumber      = 0x03,

                .bNumConfigurations = 0x01
        };

// Invoked when received GET DEVICE DESCRIPTOR
// Application return pointer to descriptor
uint8_t const *tud_descriptor_device_cb(void) {
    return (uint8_t const *) &desc_device;
}

//--------------------------------------------------------------------+
// HID Report Descriptor
//--------------------------------------------------------------------+
//TODO: setup HID report for telephony page device: https://github.com/hathach/tinyusb/blob/master/src/class/hid/hid_device.h
//Use defines from https://github.com/hathach/tinyusb/blob/master/src/class/hid/hid.h
// try logging: https://github.com/hathach/tinyusb/blob/master/docs/getting_started.md
// Make sure to report HOOK_SWITCH_HID and HOOK_OFF_LED
// Ask help in the discussion for telephony setup?https://github.com/hathach/tinyusb/discussions

//HID_USAGE_PAGE ( HID_USAGE_PAGE_DESKTOP     )                    ,\
            HID_USAGE      ( HID_USAGE_DESKTOP_KEYBOARD )                    ,\
            HID_COLLECTION ( HID_COLLECTION_APPLICATION )                    ,\
                /* Report ID if any */\
                HID_REPORT_ID(REPORT_ID_KEYBOARD) \
                /* 8 bits Modifier Keys (Shfit, Control, Alt) */ \
                HID_USAGE_PAGE ( HID_USAGE_PAGE_KEYBOARD )                     ,\
                    HID_USAGE_MIN    ( 224                                    )  ,\
                    HID_USAGE_MAX    ( 231                                    )  ,\
                    HID_LOGICAL_MIN  ( 0                                      )  ,\
                    HID_LOGICAL_MAX  ( 1                                      )  ,\
                    HID_REPORT_COUNT ( 8                                      )  ,\
                    HID_REPORT_SIZE  ( 1                                      )  ,\
                    HID_INPUT        ( HID_DATA | HID_VARIABLE | HID_ABSOLUTE )  ,\
                    /* 8 bit reserved */ \
                    HID_REPORT_COUNT ( 1                                      )  ,\
                    HID_REPORT_SIZE  ( 8                                      )  ,\
                    HID_INPUT        ( HID_CONSTANT                           )  ,\
                    /* 6-byte Keycodes */ \
                    HID_USAGE_PAGE ( HID_USAGE_PAGE_KEYBOARD )                     ,\
                        HID_USAGE_MIN    ( 0                                   )     ,\
                        HID_USAGE_MAX    ( 255                                 )     ,\
                        HID_LOGICAL_MIN  ( 0                                   )     ,\
                        HID_LOGICAL_MAX  ( 255                                 )     ,\
                        HID_REPORT_COUNT ( 6                                   )     ,\
                        HID_REPORT_SIZE  ( 8                                   )     ,\
                        HID_INPUT        ( HID_DATA | HID_ARRAY | HID_ABSOLUTE )     ,\
                   HID_USAGE_PAGE  ( HID_USAGE_PAGE_LED                   )       , /*Attemptint to get led page until 24 that includes the telephony ones*/ \
                       HID_USAGE_MIN    ( 1                                       ) ,\
                       HID_USAGE_MAX    ( 5                                       ) ,\
                       HID_REPORT_COUNT ( 5                                       ) ,\
                       HID_REPORT_SIZE  ( 1                                       ) ,\
                       HID_OUTPUT       ( HID_DATA | HID_VARIABLE | HID_ABSOLUTE  ) ,\
                       /* led padding */ \
                       HID_REPORT_COUNT ( 1                                       ) ,\
                       HID_REPORT_SIZE  ( 3                                       ) ,\
                       HID_OUTPUT       ( HID_CONSTANT                            ) ,\
            HID_COLLECTION_END                                                      ,\

uint8_t const desc_hid_report[] =
        {
            0x06, 0x00, 0xFF, /* USAGE_PAGE (Custom UC Display)  */ \
            HID_USAGE      ( 0x01 )                    ,\
            HID_COLLECTION ( HID_COLLECTION_APPLICATION )                    ,\
                /* Report ID if any */\
                HID_USAGE_MIN    ( 0x40                                    )  ,\
                HID_USAGE_MAX    ( 0x40                                    )  ,\
                HID_LOGICAL_MIN  ( 0                                      )  ,\
                0x26, 0xFF, 0x00, /* Logical Maximum (255) */ \
                HID_REPORT_SIZE  ( 8                                      )  ,\
                HID_REPORT_COUNT ( 64                                      )  ,\
                HID_INPUT        ( HID_DATA)  ,\
                HID_USAGE_MIN    ( 0x40                                    )  ,\
                HID_USAGE_MAX    ( 0x40                                    )  ,\
                HID_OUTPUT       ( HID_DATA ) ,\
            HID_COLLECTION_END                                                      ,\
};
uint8_t const org_desc_hid_report[] =
        {
            /*-------- LCD display ----------------------------------*/ \
            0x06, 0x99, 0xFF, /* USAGE_PAGE (Custom UC Display)  */ \
            0x09, 0x01,  /* USAGE (UC_DISPLAY) */ \
            0xA1, 0x01,  /* COLLECTION (Application) */ \
            /* ------------ Vendor Extension ------------------------*/ \
            0x0A, 0x00, 0xFF, /* USAGE (Vendor Extension) */ \
            0xa1, 0x02, /* COLLECTION (Logical) */ \
            0x85, 0x11, /* Report ID (11) */ \
            0x15, 0x00, /* Logical Minimum (0), */ \
            0x27, 0xFF, 0xFF, 0x00, 0x00, /* Logical Maximum (65535) */ \
            0x95, 0x01, /*  REPORT_COUNT (1) */ \
            0x75, 0x10, /* REPORT_SIZE (16) */ \
            0x0A, 0x01, 0xFF, /* USAGE (Vendor Id) */ \
            0xB1, 0x03, /* FEATURE (Cnst,Var,Abs) */ \
            0x26, 0xFF, 0x00, /* Logical Maximum (255) */ \
            0x95, 0x02, /* REPORT_COUNT (2) */ \
            0x75, 0x08, /*  REPORT_SIZE (8) */ \
            0x0A, 0x02, 0xFF, /* USAGE (Version) */ \
            0xB1, 0x03, /* FEATURE (Cnst,Var,Abs) */ \
            0xc0, /* END_COLLECTION*/ \
            /* ------------ Display Attributes ------------------------*/ \
            0x09, 0x20, /* USAGE (Display Attributes) */ \
            0xa1, 0x02, /* COLLECTION (Logical) */ \
            0x85, 0x12, /* Report ID (12) */
            0x09, 0x35, /*USAGE (Rows) */ \
            0x09, 0x36, /*USAGE (Columns) */ \
            0x15, 0x00, /*Logical Minimum (0), */ \
            0x26, 0xFF, 0x00, /*Logical Maximum (255) */ \
            0x95, 0x02, /*REPORT_COUNT (2) */ \
            0x75, 0x08, /* REPORT_SIZE (8) */ \
            0xB1, 0x03,/*FEATURE (Cnst,Var,Abs) */ \
            0x19, 0x81, /* USAGE_MINIMUM (first Display Property) */ \
            0x29, 0x8A, /* USAGE_MAXIMUM (last Display Property) */ \
            0x25, 0x01, /* Logical Maximum (1), */ \
            0x95, 0x0A, /* REPORT_COUNT (10) */ \
            0x75, 0x01, /* REPORT_SIZE (1) */ \
            0xB1, 0x03, /* FEATURE (Cnst,Var,Abs) */ \
            0x95, 0x01, /* REPORT_COUNT (1) */ \
            0x75, 0x06, /* REPORT_SIZE (6) */ \
            0xB1, 0x03, /* FEATURE (Const,Var,Abs) Pading 6 */ \
            0xc0,       /* END_COLLECTION */ \
            /* ------------ Display Control Report ------------------------*/ \
            0x09, 0x24,       /* USAGE (Display Control) */ \
            0xA1, 0x02,       /* COLLECTION(Logical) */ \
            0x85, 0x13,       /* Report ID (13) */ \
            0x09, 0x26,       /* USAGE (Enable Display) */ \
            0x09, 0x25,       /* USAGE (Clear Display) */ \
            0x0A, 0x10, 0xFF, /* USAGE (Back Light) */ \
            0x15, 0x00,       /* Logical Minimum (0), */ \
            0x25, 0x01,       /* Logical Maximum (1), */ \
            0x95, 0x03,       /* REPORT_COUNT (3) */ \
            0x75, 0x01,       /* REPORT_SIZE (1) */ \
            0x91, 0x02,       /* OUTPUT (Data,Var,Abs) */ \
            0x0A, 0x11, 0xFF, /* USAGE (Screen Select) */ \
            0x25, 0x0F,       /* Logical Maximum (15), */ \
            0x95, 0x01,       /* REPORT_COUNT (1) */ \
            0x75, 0x04,       /* REPORT_SIZE (4) */ \
            0x91, 0x02,       /* OUTPUT (Data,Var,Abs) */ \
            0x95, 0x01,       /* REPORT_COUNT (1) */ \
            0x75, 0x01,       /* REPORT_SIZE (1) */ \
            0x91, 0x03,       /* OUTPUT (Const,Var,Abs)  Pading 1*/ \
            0xc0,             /* END_COLLECTION */ \
            /* ------------ Character Attribute Report ------------------------*/ \
            0x09, 0x48,       /* USAGE (Character Attributes) */ \
            0xA1, 0x02,       /* COLLECTION(Logical) */ \
            0x85, 0x14,       /* Report ID (14) */ \
            0x19, 0x81,       /* USAGE_MINIMUM (Local User Name) */ \
            0x29, 0x8A,       /* USAGE_MAXIMUM (Number) */ \
            0x15, 0x01,       /* LOGICAL_MINIMUM (1) */ \
            0x25, 0x0A,       /* LOGICAL_MAXIMUM (10) */ \
            0x95, 0x01,       /* REPORT_COUNT (1) */ \
            0x75, 0x04,       /* REPORT_SIZE (4) */ \
            0x91, 0x00,       /* OUTPUT (Data,Ary,Abs) */ \
            0x95, 0x01,       /* REPORT_COUNT (1) */ \
            0x75, 0x04,       /* REPORT_SIZE (4) */ \
            0x91, 0x03,       /* OUTPUT (Const,Var,Abs) Pading 4 */ \
            0x95, 0x01,       /* REPORT_COUNT (1) */ \
            0x75, 0x07,       /* REPORT_SIZE (7) */ \
            0x91, 0x03,       /* OUTPUT (Const,Var,Abs) Pading 7 */ \
            0x0A, 0x22, 0xFF, /* USAGE (Text String Followed) */ \
            0x25, 0x01,       /* Logical Maximum (1), */ \
            0x95, 0x01,       /* REPORT_COUNT (1) */ \
            0x75, 0x01,       /* REPORT_SIZE (1) */ \
            0x91, 0x02,       /* OUTPUT (Data,Var,Abs) */ \
            0xc0,             /*END_COLLECTION */ \
            /* ------------ Character Report ------------------------*/ \
            0x09, 0x2B,                   /* USAGE (Character Report) */ \
            0xA1, 0x02,                   /* COLLECTION(Logical) */ \
            0x85, 0x15,                   /* Report ID (15) */ \
            0x95, 0x01,                   /* REPORT_COUNT (1) */ \
            0x75, 0x07,                   /* REPORT_SIZE (7) */ \
            0x91, 0x03,                   /* OUTPUT (Const,Var,Abs)            //Pading 7 */ \
            0x0A, 0x24, 0xFF,             /* USAGE (Last Character Report) */ \
            0x15, 0x00,                   /* Logical Minimum (0), */ \
            0x25, 0x01,                   /* Logical Maximum (1), */ \
            0x95, 0x01,                   /* REPORT_COUNT (1) */ \
            0x75, 0x01,                   /* REPORT_SIZE (1) */ \
            0x91, 0x02,                   /* OUTPUT (Data,Var,Abs) */ \
            0x0A, 0x2C, 0xFF,             /* USAGE (Display Text Data) */ \
            0x27, 0xFF, 0xFF, 0x00, 0x00, /* Logical Maximum (65535) */ \
            0x95, 0x08,                   /* REPORT_COUNT (8)            // 8 unicode char */ \
            0x75, 0x10,                   /* REPORT_SIZE (16) */ \
            0x91, 0x02,                   /* OUTPUT (Data,Var,Abs) */ \
            0xc0,                         /*            END_COLLECTION */ \
            /*--------- Icons Report ------------------------------------ */ \
            0x0A, 0x17, 0xFF, /* USAGE (Icons Control) */ \
            0xA1, 0x02,       /* COLLECTION(Logical) */ \
            0x85, 0x16,       /* Report ID (16) */ \
            0x0A, 0x18, 0xFF, /* USAGE (Presence Icon) */ \
            0x15, 0x00,       /* Logical Minimum (0), */ \
            0x25, 0x0F,       /* Logical Maximum (15), */ \
            0x95, 0x01,       /* REPORT_COUNT (1) */ \
            0x75, 0x04,       /* REPORT_SIZE (4) */ \
            0x91, 0x02,       /* OUTPUT (Data,Var,Abs) */ \
            0x95, 0x01,       /* REPORT_COUNT (1) */ \
            0x75, 0x04,       /* REPORT_SIZE (4) */ \
            0x91, 0x03,       /* OUTPUT (Const,Var,Abs) Pading 4 */ \
            0x0A, 0x1A, 0xFF, /* USAGE (Call Forward Icon) */ \
            0x0A, 0x1B, 0xFF, /* USAGE (Voicemail Icon) */ \
            0x0A, 0x1F, 0xFF, /* USAGE (Speaker Icon)??? */ \
            0x0A, 0x20, 0xFF, /* USAGE (Missed Call Icon)??? */ \
            0x25, 0x01,       /* Logical Maximum (1), */ \
            0x95, 0x04,       /* REPORT_COUNT (4) */ \
            0x75, 0x01,       /* REPORT_SIZE (1) */ \
            0x91, 0x02,       /* OUTPUT (Data,Var,Abs) */ \
            0x95, 0x01,       /* REPORT_COUNT (1) */ \
            0x75, 0x02,       /* REPORT_SIZE (2) */ \
            0x15, 0xff,       /* LOGICAL_MINIMUM (-1) */ \
            0x25, 0x01,       /* LOGICAL_MAXIMUM (1) */ \
            0x0A, 0x1C, 0xFF, /* USAGE (Mute Icon) */ \
            0x91, 0x26,       /* OUTPUT (Data, Variable, Relative, No Preferred) 2 bitsd --> output */ \
            0x95, 0x01,       /* REPORT_COUNT (1) */ \
            0x75, 0x02,       /* REPORT_SIZE (2) */ \
            0x91, 0x03,       /* OUTPUT (Const,Var,Abs) //Pading 2 */ \
            0xc0,             /* END_COLLECTION */ \
            /*--------- Display Setup Info Report ------------------------------------ */ \
            0xA1, 0x02,                   /* COLLECTION(Logical)*/ \
            0x85, 0x17,                   /* Report ID (17)*/ \
            0x15, 0x00,                   /* Logical Minimum (0),*/ \
            0x27, 0xFF, 0xFF, 0x00, 0x00, /*Logical Maximum (65535)*/ \
            0x95, 0x01,                   /* REPORT_COUNT (1)*/ \
            0x75, 0x10,                   /* REPORT_SIZE (0x10, 16)*/ \
            0x09, 0x64,                   /* USAGE (Language ID Report)*/ \
            0xB1, 0x02,                   /* FEATURE (Data, Variable, Absolute)*/ \
            0x26, 0xFF, 0x00,             /* Logical Maximum (255)*/ \
            0x95, 0x02,                   /* REPORT_COUNT (2)*/ \
            0x75, 0x08,                   /* REPORT_SIZE (8)*/ \
            0x09, 0x65,                   /* USAGE (OC Version)*/ \
            0xB1, 0x02,                   /* FEATURE (Data, Variable, Absolute)*/ \
            0xc0,                         /* 0xc0 // END_COLLECTION*/  \
            0xc0                         /* 0xc0 // END_COLLECTION*/
        };

//            HID_USAGE_PAGE ( 0xFF99      )                   , /*Set vendor extension page */ \
//            HID_USAGE      ( 0x01     )                   ,   /*trying 0x01 =  UC_Display*/ \
//            HID_COLLECTION ( HID_COLLECTION_APPLICATION  )                   ,   /*TODO */ \
//                /* Vendor Extension */\
//                HID_USAGE      ( 0xFF00 )                   ,/*Trying 0x01 = Phone */ \
//                HID_COLLECTION ( HID_COLLECTION_LOGICAL   )                   ,\
//                    HID_REPORT_ID(0x11)                               /* Set Telephone ID */ \
//                    HID_USAGE_MIN   ( 0                                      ) ,\
//                    HID_USAGE_MAX   ( 65535                                      ) ,\
//                    HID_REPORT_COUNT( 1                                      ) ,\
//                    HID_REPORT_SIZE ( 16                                      ) ,\
//                    HID_USAGE      ( 0xFF01 )                   ,/*Trying 0x01 = Phone */ \
//                    HID_FEATURE   (0x03)                    , \
//                HID_COLLECTION_END                                            , \
//                /* Display Attributes */ \
//                HID_USAGE      ( 0x20 )                   ,/*Trying 0x01 = Phone */ \
//                HID_COLLECTION ( HID_COLLECTION_LOGICAL   )                   ,\
//                    HID_REPORT_ID(0x12)                               /* Set Telephone ID */ \
//                    HID_USAGE      ( 0x35 )                   ,/*0x35 - rows */ \
//                    HID_USAGE      ( 0x36 )                   ,/*0x36 - columns */ \
//                    HID_LOGICAL_MIN ( 0                                   )  ,\
//                    HID_LOGICAL_MAX ( 255                                   )  ,\
//                    HID_REPORT_COUNT( 2                                      ) ,\
//                    HID_REPORT_SIZE ( 8                                      ) ,\
//                    HID_FEATURE   (0x03)                    , \
//                    HID_USAGE_MIN   ( 0x81                                      ) ,\
//                    HID_USAGE_MAX   ( 0x8A                                      ) ,\
//                    HID_LOGICAL_MAX ( 1                                   )  ,\
//                    HID_REPORT_COUNT( 10                                      ) ,\
//                    HID_REPORT_SIZE ( 1                                      ) ,\
//                    HID_FEATURE   (0x03)                    , \
//                    HID_REPORT_COUNT( 1                                      ) ,\
//                    HID_REPORT_SIZE ( 6                                      ) ,\
//                    HID_FEATURE   (0x03)                    , \
//                HID_COLLECTION_END                                            , \
//                /* Display Control Report */ \
//
//                    HID_USAGE_PAGE  ( HID_USAGE_PAGE_BUTTON  )                   ,\
//                    HID_LOGICAL_MIN ( 0                                      ) ,\
//                    HID_LOGICAL_MAX ( 1                                      ) ,\
//                    /* Left, Right, Middle, Backward, Forward buttons */ \
//                    HID_INPUT       ( HID_DATA | HID_VARIABLE | HID_ABSOLUTE ) ,\
//                    /* 3 bit padding */ \
//                    HID_REPORT_COUNT( 1                                      ) ,\
//                    HID_REPORT_SIZE ( 3                                      ) ,\
//                    HID_INPUT       ( HID_CONSTANT                           ) ,\
//                  HID_USAGE_PAGE  ( HID_USAGE_PAGE_DESKTOP )                   ,\
//                    /* X, Y position [-127, 127] */ \
//                    HID_USAGE       ( HID_USAGE_DESKTOP_X                    ) ,\
//                    HID_USAGE       ( HID_USAGE_DESKTOP_Y                    ) ,\
//                    HID_LOGICAL_MIN ( 0x81                                   ) ,\
//                    HID_LOGICAL_MAX ( 0x7f                                   ) ,\
//                    HID_REPORT_COUNT( 2                                      ) ,\
//                    HID_REPORT_SIZE ( 8                                      ) ,\
//                    HID_INPUT       ( HID_DATA | HID_VARIABLE | HID_RELATIVE ) ,\
//                    /* Verital wheel scroll [-127, 127] */ \
//                    HID_USAGE       ( HID_USAGE_DESKTOP_WHEEL                )  ,\
//                    HID_LOGICAL_MIN ( 0x81                                   )  ,\
//                    HID_LOGICAL_MAX ( 0x7f                                   )  ,\
//                    HID_REPORT_COUNT( 1                                      )  ,\
//                    HID_REPORT_SIZE ( 8                                      )  ,\
//                    HID_INPUT       ( HID_DATA | HID_VARIABLE | HID_RELATIVE )  ,\
//                  HID_USAGE_PAGE  ( HID_USAGE_PAGE_CONSUMER ), \
//                   /* Horizontal wheel scroll [-127, 127] */ \
//                    HID_USAGE_N     ( HID_USAGE_CONSUMER_AC_PAN, 2           ), \
//                    HID_LOGICAL_MIN ( 0x81                                   ), \
//                    HID_LOGICAL_MAX ( 0x7f                                   ), \
//                    HID_REPORT_COUNT( 1                                      ), \
//                    HID_REPORT_SIZE ( 8                                      ), \
//                    HID_INPUT       ( HID_DATA | HID_VARIABLE | HID_RELATIVE ), \
//                HID_COLLECTION_END                                            , \
//            HID_COLLECTION_END

// Invoked when received GET HID REPORT DESCRIPTOR
// Application return pointer to descriptor
// Descriptor contents must exist long enough for transfer to complete
uint8_t const *tud_hid_descriptor_report_cb(void) {
    printf("DEBUG: tud_hid_descriptor_report_cb triggered\n");
    return desc_hid_report;
}

//--------------------------------------------------------------------+
// Configuration Descriptor
//--------------------------------------------------------------------+

enum {
    ITF_NUM_HID,
    ITF_NUM_TOTAL
};

#define  CONFIG_TOTAL_LEN  (TUD_CONFIG_DESC_LEN + TUD_HID_DESC_LEN)

#define EPNUM_HID   0x81

uint8_t const desc_configuration[] =
        {
                // Config number, interface count, string index, total length, attribute, power in mA
                TUD_CONFIG_DESCRIPTOR(1, ITF_NUM_TOTAL, 0, CONFIG_TOTAL_LEN, TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP, 100),

                // Interface number, string index, protocol, report descriptor len, EP In & Out address, size & polling interval
                TUD_HID_DESCRIPTOR(ITF_NUM_HID, 0, HID_PROTOCOL_NONE, sizeof(desc_hid_report), EPNUM_HID,
                                   CFG_TUD_HID_BUFSIZE, 10)
        };

// Invoked when received GET CONFIGURATION DESCRIPTOR
// Application return pointer to descriptor
// Descriptor contents must exist long enough for transfer to complete
uint8_t const *tud_descriptor_configuration_cb(uint8_t index) {
    printf("DEBUG: tud_descriptor_configuration_cb triggered\n");
    printf("index: %X\n", index);
    (void) index; // for multiple configurations
    return desc_configuration;
}

//--------------------------------------------------------------------+
// String Descriptors
//--------------------------------------------------------------------+

// array of pointer to string descriptors
char const *string_desc_arr[] =
        {
                //(const char[]) {0x00, 0x00}, // 0: is supported language is English (0x0409)
                (const char[]) {0x00}, // 0: is supported language is English (0x0409)
                "PLENOM APS",                     // 1: Manufacturer
                "BUSYLIGHT",              // 2: Product
                "0",                      // 3: Serials, should use chip ID
        };

static uint16_t _desc_str[32];

// Invoked when received GET STRING DESCRIPTOR request
// Application return pointer to descriptor, whose contents must exist long enough for transfer to complete
uint16_t const *tud_descriptor_string_cb(uint8_t index, uint16_t langid) {
    printf("DEBUG: tud_descriptor_string_cb triggered\n");
    printf("index: %X\n", index);
    printf("langid: %X\n", langid);
    (void) langid;

    uint8_t chr_count;

    if (index == 0) {
        memcpy(&_desc_str[1], string_desc_arr[0], 2);
        chr_count = 1;
    } else {
        // Convert ASCII string into UTF-16

        if (!(index < sizeof(string_desc_arr) / sizeof(string_desc_arr[0]))) return NULL;

        const char *str = string_desc_arr[index];

        // Cap at max char
        chr_count = strlen(str);
        if (chr_count > 31) chr_count = 31;

        for (uint8_t i = 0; i < chr_count; i++) {
            _desc_str[1 + i] = str[i];
        }
    }

    // first byte is length (including header), second byte is string type
    _desc_str[0] = (TUSB_DESC_STRING << 8) | (2 * chr_count + 2);

    return _desc_str;
}
