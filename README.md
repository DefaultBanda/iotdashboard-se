# ESP32 Weather Clock with ILI9341 Display

This project is a simple weather and clock display built with an ESP32 and a 2.8" ILI9341 TFT display. It connects to WiFi to retrieve the current time via an NTP (Network Time Protocol) server and fetches weather information from OpenWeatherMap. The time and weather are displayed on the TFT screen.

## Features

- Displays the current time and date in a readable format.
- Shows the current weather and temperature using data from OpenWeatherMap.
- Customizable UTC offset for accurate local time display.
- Auto-updates time and weather at specified intervals.
![IMG_3883](https://github.com/user-attachments/assets/0d7ff5a7-62cc-4668-b9e7-a589988d2dee)

https://github.com/user-attachments/assets/161a7464-dd6c-4200-98d6-f24ae300610d


## Components

- **ESP32 Development Board**
- **2.8" ILI9341 TFT Display (with SPI interface)**
- **Internet connection (WiFi)**

## Wiring

| ILI9341 Pin | ESP32 Pin  |
|-------------|------------|
| VCC         | 3.3V       |
| GND         | GND        |
| CS          | GPIO 17    |
| RESET       | GPIO 5     |
| D/C         | GPIO 16    |
| SD_MOSI     | GPIO 23    |
| SD_SCK      | GPIO 18    |
| LED         | GPIO 32    |
| SD_MISO     | GPIO 19    |
| T_CLK       | GPIO 18    |
| T_CS        | GPIO 21    |
| T_DI        | GPIO 23    |
| T_DO        | GPIO 19    |
| SD_CS       | GPIO 12    |

## Setup Instructions

1. **Open the project in the Arduino IDE.**

2. **Install the required libraries:**
   - Adafruit GFX Library
   - Adafruit ILI9341
   - ArduinoJson
   - NTPClient (already included with ESP32 board setup)

3. **Update WiFi credentials and OpenWeatherMap API key in the code:**
![image](https://github.com/user-attachments/assets/0c066be9-f092-41dc-a551-7327161e80b0)

   ```cpp
  ssid = "YOUR_SSID";
  password = "YOUR_PASSWORD";
  apiKey = "YOUR_API_KEY";


  
   

