# 3D Printer Control MKII
This is the GitHub repository for the project: https://buildcomics.com/ (TO Be Determined) \
You will also find the "instructions"  there!

## Models
Find all the models for the keyboard here: (TODO)

## Main Code
The main code is based on Tinyusb: https://github.com/hathach/tinyusb \
The device should mimic a telephony HID device, as specified by Microsoft teams here: https://docs.microsoft.com/en-us/skypeforbusiness/certification/test-spec \
The actual document with the specs is in this zip file: https://download.microsoft.com/download/9/f/6/9f63c68d-817b-40ff-935c-9dd8b74cb07d/V4%20Release.zip  \
file name: Teams_DevicesGeneralSpecification_4_0_final.pdf
\
also interesting, the SkypeforBusiness_CallDisplaySpecification_3_0_Final.pdf has custom HID usage pages for display setup. This could be more usefull to make a busylight of. \ I'm planning on getting my USB busylight from the office to steal it's HID Descriptors and see what they do.

More usefull explanation here: https://eleccelerator.com/tutorial-about-usb-hid-report-descriptors/

\

And background spec: https://www.usb.org/hid

\

And a more readable explanation of this: https://docs.microsoft.com/en-us/windows-hardware/drivers/hid/hid-usages

## Testing
Command to test HID descriptor of file: \
`sudo usbhid-dump -a 1:58 | grep -v : | xxd -r  -p | hidrd-convert -o spec`
\
using the hidrd tool from: https://github.com/DIGImend/hidrd

## Wiring
TODO

## Raspberry Pi Pico
Assuming you have the raspberry pi pico c SDK installed(https://github.com/raspberrypi/pico-sdk) \
The following (standard) commands will create "main.uf2" that can be copied onto the raspberry pi in bootsel mode:
1. git clone git@github.com:buildcomics/mst_keyboard.git
2. cd mst_keyboard
3. mkdir build
4. cd build
5. cmake ..
6. make
7. Now press the button on your raspbery pi pico, connect it and copy the main.uf2 to the pico that should show up ass a mass storage device

usb urb request:
```
char packet_bytes[] = {
  0x8f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x06, 0x04, 0x55, 0xff, 0xff, 0xff, 0x03, 0xeb
};
```
Response:
```
char packet_bytes[] = {
  0x30, 0x30, 0x30, 0x31, 0x50, 0x4c, 0x45, 0x4e,
  0x4f, 0x4d, 0x30, 0x30, 0x30, 0x30, 0x30, 0x31,
  0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x31,
  0x44, 0x41, 0x53, 0x41, 0x4e, 0x30, 0x30, 0x30,
  0x32, 0x30, 0x31, 0x35, 0x30, 0x35, 0x32, 0x38,
  0x30, 0x32, 0x31, 0x30, 0x30, 0x30, 0x30, 0x30,
  0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
  0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30
};
```

## USB Pcap in "pico and kunado.pcap"
- Pico start: 40.85, source 2.10.0, wireshark filter: usb.device_address == 10
- Kuando start: 71.89, source 2.11.0, wireshark filter: usb.device_address == 11

# Debug OUTPUT
Debug output of the same thing is stored in "pico usb debug output.txt"
Working with setting of debug = 2

## License
MIT License

Copyright (c) 2021 buildcomics

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
