# Core2GuiDemo
This repository contains the code for a smart home application that controls lights using an M5Stack Core2 device. The application is primarily written in C++ and includes a Telegram bot for synchronizing light states via the MQTT protocol.

## Repository Structure
The repository is divided into two main folders:

`app`: This folder contains the main part of the application, which is written in C++. It controls the M5Stack Core2 device that manages the lights in a home.

`telegram`: This folder contains the code for a Telegram bot. The bot is used to synchronize the light states with the main application via the MQTT protocol.

### App
The 'app' folder, besides the C++ source, contains the following files:

- .gitignore: Specifies intentionally untracked files to ignore.
- README.md: Provides information about the project.
- platformio.ini: The PlatformIO configuration file.
The main application is built using the PlatformIO tool with the ESP32 platform and the Arduino framework. The platformio.ini file specifies the board type (m5stack-core2), the platform (espressif32), and the framework (arduino). It also lists the library dependencies, which include M5Core2, lv_arduino, pubsubclient, and FastLED.

### Telegram
The 'telegram' folder contains the following files:

- .gitignore: Specifies intentionally untracked files to ignore.
- README.md: Provides information about the project.
- requirements.txt: Lists the Python package dependencies for the Telegram bot.
- main.py: The main script for the Telegram bot.
- chatbot.py: Handles the chat functionality of the Telegram bot.
- lightmanager.py: Manages the light states.
- tempmanager.py: Manages temperature data.
- config.template.py: A template for configuration settings.
The Telegram bot is written in Python and requires the paho-mqtt and python-telegram-bot packages, as specified in the requirements.txt file.

## Quickstart
To get started with this project, clone the repository and install the necessary dependencies. For the main application, you will need PlatformIO and the libraries specified in the platformio.ini file. For the Telegram bot, you will need Python and the packages listed in the requirements.txt file.
```
git clone [https://github.com/alptbz/core2guidemo](https://github.com/xRemo7/core2guidemo.git)
```
 - Open repository with PlatformIO
 - rename `src/config.cpp.template` to `src/config.cpp`
 - Adjust serial port in `platform.ini`
 - Build and upload

## Links
 - ToF Sensor Library: https://registry.platformio.org/libraries/pololu/VL53L0X/installation
 - Tof Sensor Example: https://github.com/pololu/vl53l0x-arduino/blob/master/examples/Continuous/Continuous.ino
 - Used LVGL Example: https://github.com/mhaberler/m5core2-lvgl-demo
 - LVGL 7.x Docs: https://docs.lvgl.io/7.11/
 - Original chinese source: https://macsbug.wordpress.com/2021/06/18/how-to-run-lvgl-on-m5stack-esp32/
 - FastLED Example: https://github.com/m5stack/M5-ProductExampleCodes/blob/master/Base/M5GO_BOTTOM2/M5GO_BOTTOM2.ino

## More examples
 - https://github.com/alptbz/core2guitofexample/
 - https://github.com/alptbz/core2numpad/


## License
This project is open source.

