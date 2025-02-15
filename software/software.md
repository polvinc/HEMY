# Software/Firmware Configuration
## Introduction
HEM*Y* uses the [OnStepX](https://github.com/hjd1964/OnStepX) ([wiki](https://groups.io/g/onstep/wiki/home)) computerized telescope control firmware, a free and open-source project released under the GPL version 3.  You'll need to compile and flash this firmware to your main board following the instructions below.  These instructions are adapted from the [wiki](https://onstep.groups.io/g/main/wiki/32777), where you can find more detail if desired.

## Install the Arduino IDE
You'll compile the firmware using the latest version of the Arduino IDE, which is available for Windows, macOS, and Linux.  Download it from https://www.arduino.cc/en/software and install it, if it isn't already installed.

## Install Teensy Support
The TeenAstro Redux board uses a Teensy 4.0 on a MicroMod card, so you'll need to install Teensy support in the Arduino IDE.  To do this, in the Arduino IDE, go to File -> Preferences (Arduino IDE -> Settings under macOS), and enter `https://www.pjrc.com/teensy/package_teensy_index.json` in the **Additional Boards Manager URLs** field.  If that field isn't empty, separate any URLs with commas, like this: `https://www.pjrc.com/teensy/package_teensy_index.json,https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`.  Then click OK.  Then go to Tools -> Board -> Boards Manager and install Teensy support.

## Install Support Libraries
You'll next need to install libraries to support some of the devices you'll be using.  Go to Tools -> Manage Libraries and install the following libraries:
* Makuna RTC, version 2.3.5
* Time, version 1.6.1
* TinyGPSPlusPlus, latest version
+ TMCStepper

## Download the OnStepX Code
Browse to https://github.com/hjd1964/OnStepX to download the OnStepX code.  As of this writing (15 Feb 25), the latest release published is 10.24c, and you must use at least version 10.24i.  If there's a release more recent than 10.24i on the Releases page, you can download and use that.  Otherwise, click the green **Code** button to download a `.zip` file of the repository.  Unzip, and rename its folder to `OnStepX` (it will likely default to `OnStepX-main`).

Also, download the [Config.h](Config.h) file from this repository.  Place it in the `OnStepX` folder, overwriting the copy that's already there.

## Flash the Firmware
You'll need to prepare your hardware.  Install the Teensy MicroMod onto the main board and secure it in place with an M3x6 screw.  Connect the board to 12-24V DC power.  Plug in a USB cable from your computer to the board.  Turn on the power switch on the board.  The white LED on the board should turn on, and after a moment, the blue LED on the Teensy should also turn on.  The Arduino IDE should recognize that a `Teensy MicroMod` is connected.

To compile and flash the firmware, either click the Upload button in the code window (the arrow in the upper-left corner), or go to Sketch -> Upload.  Compilation and uploading will take a few moments, after which the Teensy will reset.

## Test
Connect to your controller using the Serial Monitor in the Arduino IDE, and type the following command: `:GVP#`, then press Enter.  It should respond with `On-Step#`.

## ESP-01S
Unknown what, if anything, needs to be done with respect to the WiFi device.

## Hand Controller
TODO