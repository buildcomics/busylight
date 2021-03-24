#include <stdio.h>
#include <stdio.h>
#include <string.h>

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"

#include "bsp/board.h"
#include "tusb.h"

#include "usb_descriptors.h"

#define BTN_1_GPIO 9
#define BTN_2_GPIO 4
#define LED_1_RED_GPIO 8
#define LED_1_GREEN_GPIO 10
#define LED_1_BLUE_GPIO 11

#define EVENT_MASK_LOW 0x1
#define EVENT_MASK_HIGH 0x2
#define PWM_COUNT_TOP 100

//Function Prototypes
void btn_callback(uint gpio, uint32_t events);
void hid_task(void);

int main() {
    stdio_init_all(); //Initialize debug interface
    printf("DEBUG: starting up buildcomics HID device...\n");

    board_init();
    tusb_init();
    printf("DEBUG: HID Device initialized\n");

    //Setup GPIO with callback:
    gpio_pull_up(BTN_1_GPIO); //Enable pullup on button 1 IO
    gpio_pull_up(BTN_2_GPIO); //Enable pullup on button 2 IO
    gpio_set_irq_enabled_with_callback(BTN_1_GPIO, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &btn_callback); //Set IRQ interrupt when switch has rising edge
    gpio_set_irq_enabled_with_callback(BTN_2_GPIO, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &btn_callback); //Set IRQ interrupt when switch has rising edge

    //setup LED gpio
    //gpio_init(LED_1_RED_GPIO);
    //gpio_set_dir(LED_1_RED_GPIO, GPIO_OUT);

    //setup LED PWM
     // Configure PWM slice and set it running
    pwm_config cfg = pwm_get_default_config();
    pwm_config_set_wrap(&cfg, PWM_COUNT_TOP);
    pwm_init(pwm_gpio_to_slice_num(LED_1_RED_GPIO), &cfg, true);
    pwm_init(pwm_gpio_to_slice_num(LED_1_GREEN_GPIO), &cfg, true);
    pwm_init(pwm_gpio_to_slice_num(LED_1_BLUE_GPIO), &cfg, true);

    // Note we aren't touching the other pin yet -- PWM pins are outputs by
    // default, but change to inputs once the divider mode is changed from
    // free-running. It's not wise to connect two outputs directly together!
    gpio_set_function(LED_1_RED_GPIO, GPIO_FUNC_PWM);
    gpio_set_function(LED_1_GREEN_GPIO, GPIO_FUNC_PWM);
    gpio_set_function(LED_1_BLUE_GPIO, GPIO_FUNC_PWM);

    pwm_set_gpio_level(LED_1_RED_GPIO, 1 * (PWM_COUNT_TOP + 1));
    sleep_ms(500);
    pwm_set_gpio_level(LED_1_RED_GPIO, 0 );
    pwm_set_gpio_level(LED_1_GREEN_GPIO, 1 * (PWM_COUNT_TOP + 1));
    sleep_ms(500);
    pwm_set_gpio_level(LED_1_GREEN_GPIO, 0 );
    pwm_set_gpio_level(LED_1_BLUE_GPIO, 1 * (PWM_COUNT_TOP + 1));
    // MAIN LOOP
    while (true){
        hid_task();
        tud_task(); // tinyusb device task
    }
    return 0;
}

// Invoked when device is mounted
void tud_mount_cb(void) {
    printf("DEBUG: MOUNTED\n");
}

// Invoked when device is unmounted
void tud_umount_cb(void) {
    printf("DEBUG: Unmounted\n");
}

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en) {
    (void) remote_wakeup_en;
    printf("DEBUG: Suspended\n");
}

// Invoked when usb bus is resumed
void tud_resume_cb(void) {
    printf("DEBUG: Resumed Mounted\n");
}

//--------------------------------------------------------------------+
// USB HID
//--------------------------------------------------------------------+
void hid_task(void) {
    // Poll every 10ms
    const uint32_t interval_ms = 10;
    static uint32_t start_ms = 0;

    if (board_millis() - start_ms < interval_ms) return; // not enough time
    start_ms += interval_ms;

    uint32_t const btn = 1;

    // Remote wakeup
    if (tud_suspended() && btn) {
        // Wake up host if we are in suspend mode
        // and REMOTE_WAKEUP feature is enabled by host
        tud_remote_wakeup();
    }


}

// Invoked when received control request with VENDOR TYPE
//TU_ATTR_WEAK bool tud_vendor_control_request_cb(uint8_t rhport, tusb_control_request_t const * request);
bool tud_vendor_control_request_cb(uint8_t rhport, tusb_control_request_t const * request) {
    // TODO not Implemented
    printf("DEBUG: tud_vendor_control_request_cb triggered\n");
    (void) rhport;
    (void) request;

    return 0;
}

// Invoked when vendor control request is complete
//TU_ATTR_WEAK bool tud_vendor_control_complete_cb(uint8_t rhport, tusb_control_request_t const * request);
bool tud_vendor_control_complete_cb(uint8_t rhport, tusb_control_request_t const * request) {
    // TODO not Implemented
    printf("DEBUG: tud_vendor_control_complete_cb triggered\n");
    (void) rhport;
    (void) request;

    return 0;
}




//attempt to use:
// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(uint8_t report_id, hid_report_type_t report_type, uint8_t *buffer, uint16_t reqlen) {
    // TODO not Implemented
    printf("DEBUG: tud_hid_get_report_cb triggered\n");
    (void) report_id;
    (void) report_type;
    (void) buffer;
    (void) reqlen;

    return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t report_id, hid_report_type_t report_type, uint8_t const *buffer, uint16_t bufsize) {
    printf("DEBUG: tud_hid_set_report_cb triggered\n");
    printf("DEBUG: report_id: %X\n", report_id);
    printf("DEBUG: report_type: %X\n", report_type);
    printf("DEBUG: bufsize: %d\n", bufsize);
    const char setup_request_string[] = {
        0x8f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x06, 0x04, 0x55, 0xff, 0xff, 0xff, 0x03, 0xeb
    };

    if (strcmp(buffer, setup_request_string) == 0) {
        printf("DEBUG: Matching setup request string, answering\n");
        const char setup_request_return[] = {
            0x30, 0x30, 0x30, 0x31, 0x50, 0x4c, 0x45, 0x4e,
            0x4f, 0x4d, 0x30, 0x30, 0x30, 0x30, 0x30, 0x31,
            0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x31,
            0x44, 0x41, 0x53, 0x41, 0x4e, 0x30, 0x30, 0x30,
            0x32, 0x30, 0x31, 0x35, 0x30, 0x35, 0x32, 0x38,
            0x30, 0x32, 0x31, 0x30, 0x30, 0x30, 0x30, 0x30,
            0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
            0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30
        };
        tud_hid_report(0, &setup_request_return, sizeof(setup_request_return));
    }
    else if(bufsize == 64 && (buffer[0] == 0x10 || buffer[0] == 0x11)) {
        printf("DEBUG light command received:\n");
        printf("DEBUG RED: %d \n", buffer[2]); //2 = red pwm value
        printf("DEBUG GREEN: %d \n", buffer[3]); //3 = green pwm value
        printf("DEBUG BLUE: %d \n", buffer[4]); //4 = blue pwm value
        pwm_set_gpio_level(LED_1_RED_GPIO, buffer[2]);
        pwm_set_gpio_level(LED_1_GREEN_GPIO, buffer[3]);
        pwm_set_gpio_level(LED_1_BLUE_GPIO, buffer[4]);
    }
    else {
        printf("DEBUG, not matching setup string BUFFER CONTENT:\n");
        for (int i = 0; i < bufsize; i++) {
            printf("%02X ", buffer[i]);
        }
        printf("\n - End \n");
    }

    (void) report_id;
    (void) report_type;
    (void) buffer;
    (void) bufsize;
}


void btn_callback(uint gpio, uint32_t events) {
    printf("DEBUG: btn_callback triggered\n");
    printf("DEBUG: gpio: %d\n", gpio);
    printf("DEBUG: events: %d\n", events);

    if (events == GPIO_IRQ_EDGE_FALL && gpio == BTN_1_GPIO) {
        printf("DEBUG: Falling Edge on Button 1\n");
        gpio_put(LED_1_RED_GPIO, 1); //Turn on the LED
        /*------------- Keyboard -------------*/
        if (tud_hid_ready()) {
            uint8_t keycode[6] = {0};
            keycode[0] = HID_KEY_A;
            tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, keycode);
        }
    }
    else if (events == GPIO_IRQ_EDGE_FALL && gpio == BTN_2_GPIO) {
        if (tud_hid_ready()) {
            printf("DEBUG: Falling Edge on Button 2\n");
//            tud_hid_mouse_report(REPORT_ID_TELEPHONE, 0x00, 5, 5, 0, 0); //move down and right with delta 5
        }
    }
    else {
        gpio_put(LED_1_RED_GPIO, 0); //Turn off the LED
        if (tud_hid_ready()) {
            tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, NULL);
        }
    }
}
