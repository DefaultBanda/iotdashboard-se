# ESP32 Weather Dashboard

This project is an ESP32-based dashboard that displays weather, temperature, air quality index (AQI), humidity, and wind speed on an ILI9341 TFT display. It fetches data from OpenWeatherMap and synchronizes the time via NTP.

## Features
- Real-time weather information
- Cycles through multiple screens every 5 seconds
- Displays:
  - Current time and date
  - Weather, temperature, and AQI
  - Humidity and wind speed
  - Health information and outfit suggestions
  
  - ![IMG_3883](https://github.com/user-attachments/assets/f209d883-8a73-4a99-8ac3-d1dffee2d08c)
 
  - 

## Requirements
- **Hardware**: ESP32, ILI9341 TFT display, XPT2046 touchscreen
- **Libraries**:
  - `Adafruit GFX`
  - `Adafruit ILI9341`
  - `ArduinoJson`
  - `NTPClient`

## Setup
1. Clone the repository and open the `.ino` file in Arduino IDE.
2. Install required libraries from the Library Manager.
3. Update your WiFi credentials and OpenWeatherMap API key:
   
