# ESP32 OLED Clock

An ESP32-based 128×64 OLED clock with a cyan pixel-art butterfly animation and an automatically updated weekday and date display.

## Features

- Animated bitmap frames displayed on a 128×64 monochrome OLED.
- Weekday shown above the date.
- Date formatted as `DD.MM.YYYY`.
- Network time synchronization using NTP.
- India Standard Time (UTC+05:30).
- Animation frame interval set to 180 ms.

## Hardware

- ESP32 development board
- 0.96-inch 128×64 SSD1306 I²C OLED display

## Wiring

| OLED | ESP32 |
| --- | --- |
| VCC | 3.3V (or display-board-supported supply) |
| GND | GND |
| SDA | GPIO 21 |
| SCL | GPIO 22 |

The sketch uses I²C address `0x3C`.

## Arduino libraries

Install these libraries through the Arduino IDE Library Manager:

- Adafruit GFX Library
- Adafruit SSD1306

The ESP32 Arduino core provides the Wi-Fi and time functions used by the sketch.

## Setup

1. Open `ESP32_OLED_Clock.ino` in Arduino IDE.
2. Add the companion `oled_animation.h` file to the sketch.
3. Replace `YOUR_WIFI_SSID` and `YOUR_WIFI_PASSWORD` with your Wi-Fi credentials.
4. Select your ESP32 board and upload.
5. Open Serial Monitor at 115200 baud to view connection and time-sync messages.

## Project files

- `ESP32_OLED_Clock.ino` — main Arduino sketch.
- `oled_animation.h` — butterfly animation bitmap frames (companion file; to be added).

## Notes

The clock obtains its date and time from an NTP server after connecting to Wi-Fi. The displayed date is refreshed when the day changes. Keep Wi-Fi credentials private; do not commit personal credentials to a public repository.

## License

No license has been added yet.
