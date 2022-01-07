# raspberry_monitor

## Simple boot/restart monitor for the Raspberry Pi Zero W implemented through Attiny85 or Attiny88 Arduino device

Requirements:
- Arduino device (tested with an ATTINY88 Arduino device)
- SH1106 OLED display (not strictly required: comment out `#define USE_SH1106 1` if the display is not used.
- Raspberry Pi Zero W

Notice that other Raspberry devices might have a different LED ACT behaviour.

This sketch should also work with ATTINY85, Digispark, or any other Arduino device.

Also supporting the [PINN](https://github.com/procount/pinn) rescue mode.

Installation:

1. solder a wire to the LED ACT pin of the Raspberry Pi (ref. A in the image) and connect it to the LED_ACT_PIN of the Attiny device.
2. connect the RESET_PIN of the Attiny device to the pin "RUN" of the Raspberry Pi (ref. B in the image)
3. connect a SH1106 OLED device to the SDA and SCL pins of the Attiny device; notice that this display device is safe to be also connected to the Raspberry Pi
4. load the sketch to the Attiny device
4. power on the Attiny, the Raspberry Pi and the SH1106 display.

The ACT led should blink in normal operation.

If the ACT led remains switched off (shutdown) or on (crash) for more than two minutes, the Attiny device will reboot the Raspberry Pi.

If connecting the SH1106 OLED display to the Raspberry, after the boot the Raspberry can clear the screen (or use the display for other things).

Notice that when the Raspberry device is in PINN rescue mode, the device does not reboot (it appears that luckily the rescue OS embedded in the PINN software keeps the ACT led quickly blinking. Anyway, after `echo 1 >/sys/class/leds/led0/brightness` (or `echo 0 >/sys/class/leds/led0/brightness`) the Raspberry device reboots in two minutes.

The internal led of the Arduino device is used to monitor the watchdog feature; comment out `#define USE_INTERNAL_LED 1` if the usage of the internal led is not wanted.

![LED-act](https://user-images.githubusercontent.com/8292987/148591592-8e3d445f-b40c-40a1-9005-0b2cc7c86a17.png)

## Used Libraries:
- https://github.com/notisrac/SH1106Lib (No buffer SH1106 OLED display driver)
- https://playground.arduino.cc/Main/SoftwareI2CLibrary/ (required library of SH1106Lib - I2C Master)
- https://github.com/notisrac/TinyPrint (required library of SH1106Lib)
