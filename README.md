# internet_radio
This is a project to build an internet radio using Espressif ESP32-WROOM-32D microcontroller with preinstalled ESP32S.

Serial monitor baudrate can be set in platformio.ini with the "monitor_speed" variable.

# Objectives
1. Program ESP32 as a HTTP server using WiFi Station (STA) mode.
    1. Make a nice GUI
    2. Add more radio stations
2. Add a Rotary Encoder Module to change the volume
3. Add an LCD screen
4. Add a powerbank as a main supply

# ESP32
I am using Espressif ESP32-WROOM-32D Microcontroller.

### Errors
- If Platformio fails to fetch the correct address to flash the code into, hold the BOOT button on the microcontroller during "PlatformIO: Upload".
    - There is a better way of doing this from software side, so that user would not have to physically hold the BOOT button, but I lost the explanation.

# VS1053 Decoder
Example code was used from this GitHub: https://github.com/baldram/ESP_VS1053_Library

Most guides on the internet are saying to connect the VS1053 to 5V, which is not true! In VS1053 specifications, the Absolute Maximum Ratings show that power supply can't be over 3.6V.
http://www.vlsi.fi/fileadmin/datasheets/vs8053.pdf