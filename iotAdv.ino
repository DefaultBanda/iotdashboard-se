#include <WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <XPT2046_Touchscreen.h> // Touchscreen library

#define TFT_CS 17
#define TFT_DC 16
#define TFT_RST 5
#define TFT_LED 32
#define TOUCH_CS 21
#define TOUCH_IRQ 4

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
XPT2046_Touchscreen ts(TOUCH_CS, TOUCH_IRQ);

const char* ssid = "SSID";
const char* password = "PASS";
const String apiKey = "Hii";
const String city = "LOCO";
const String countryCode = "CA";
String weatherUrl = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "," + countryCode + "&appid=" + apiKey + "&units=metric";

WiFiUDP ntpUDP;
const long utcOffsetInSeconds = -18000;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds, 60000);

int currentPage = 0; // Tracks which page to display
String currentWeather = "clear";
float currentTemp = 0;
int iaq = 50; // Placeholder for IAQ
float uvIndex = 5.0; // Placeholder for UV Index
String pollenLevel = "Moderate"; // Placeholder for pollen level
float humidity = 50; // Placeholder for humidity
float windSpeed = 5; // Placeholder for wind speed
unsigned long lastWeatherUpdate = 0;
const long updateInterval = 15 * 60000; // Update weather every 15 minutes

unsigned long lastPageSwitch = 0;
const unsigned long pageSwitchInterval = 5000; // Switch pages every 5 seconds

void setup() {
  Serial.begin(115200);
  pinMode(TFT_LED, OUTPUT);
  digitalWrite(TFT_LED, HIGH);
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(ILI9341_BLACK);

  ts.begin();
  ts.setRotation(1);

  connectWiFi();
  timeClient.begin();
  updateWeather();
}

void loop() {
  timeClient.update();

  if (millis() - lastWeatherUpdate > updateInterval) {
    updateWeather();
    lastWeatherUpdate = millis();
  }

  // Automatically cycle pages every 5 seconds
  unsigned long currentMillis = millis();
  if (currentMillis - lastPageSwitch >= pageSwitchInterval) {
    lastPageSwitch = currentMillis;
    currentPage = (currentPage + 1) % 4;
    tft.fillScreen(ILI9341_BLACK);
  }

  displayPage();
}

void connectWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
}

void displayPage() {
  switch (currentPage) {
    case 0:
      displayMainInfo();
      break;
    case 1:
      displayHealthAndOutfit();
      break;
    case 2:
      displayHumidityAndWind();
      break;
    case 3:
      displayPlaceholder();
      break;
  }
}

void displayMainInfo() {
  // Display time at the top in larger font
  String timeStr = formatTime();
  tft.setTextColor(ILI9341_CYAN, ILI9341_BLACK);
  tft.setTextSize(3);
  tft.setCursor(10, 10);
  tft.print("Time: ");
  tft.print(timeStr);

  // Display date below the time
  String dateStr = formatDate();
  tft.setTextSize(2);
  tft.setCursor(10, 50);
  tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
  tft.print("Date: ");
  tft.print(dateStr);

  // Weather description and temperature below date
  tft.setCursor(10, 90);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.print("Weather: ");
  tft.setTextColor(ILI9341_CYAN, ILI9341_BLACK);
  tft.print(currentWeather);

  tft.setCursor(10, 120);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.print("Temp: ");
  tft.setTextColor(ILI9341_RED, ILI9341_BLACK);
  tft.print(String(currentTemp) + " C");

  // Display IAQ below temperature
  tft.setCursor(10, 150);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.print("IAQ: ");
  tft.setTextColor(ILI9341_ORANGE, ILI9341_BLACK);
  tft.print(String(iaq));
}

void displayHealthAndOutfit() {
  // Health Index Information
  tft.setTextColor(ILI9341_MAGENTA, ILI9341_BLACK);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.print("UV Index: ");
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.print(String(uvIndex));

  tft.setTextColor(ILI9341_MAGENTA, ILI9341_BLACK);
  tft.setCursor(10, 40);
  tft.print("Pollen: ");
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.print(pollenLevel);

  tft.setTextColor(ILI9341_MAGENTA, ILI9341_BLACK);
  tft.setCursor(10, 70);
  tft.print("AQI: ");
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.print(String(iaq));

  // Outfit Recommendation based on temperature
  String outfitRec;
  if (currentTemp > 25) {
    outfitRec = "T-shirt & shorts";
  } else if (currentTemp > 15) {
    outfitRec = "Light jacket";
  } else {
    outfitRec = "Coat & warm layer";
  }

  tft.setCursor(10, 110);
  tft.setTextColor(ILI9341_BLUE, ILI9341_BLACK);
  tft.print("Outfit: ");
  tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
  tft.print(outfitRec);
}

void displayHumidityAndWind() {
  tft.setTextColor(ILI9341_CYAN, ILI9341_BLACK);
  tft.setTextSize(2);
  tft.setCursor(10, 30);
  tft.print("Humidity: ");
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.print(String(humidity) + "%");

  tft.setCursor(10, 70);
  tft.setTextColor(ILI9341_CYAN, ILI9341_BLACK);
  tft.print("Wind Speed: ");
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.print(String(windSpeed) + " m/s");
}

void displayPlaceholder() {
  tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.print("More features coming soon!");
}

void updateWeather() {
  HTTPClient http;
  http.begin(weatherUrl);
  int httpCode = http.GET();

  if (httpCode == 200) {
    String payload = http.getString();
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, payload);

    currentWeather = doc["weather"][0]["description"].as<String>();
    currentTemp = doc["main"]["temp"].as<float>();
    iaq = doc["main"]["aqi"].as<int>(); // Placeholder for AQI
    humidity = doc["main"]["humidity"].as<float>();
    windSpeed = doc["wind"]["speed"].as<float>();

    Serial.println("Weather: " + currentWeather);
    Serial.println("Temperature: " + String(currentTemp) + " C");
  } else {
    Serial.println("Failed to retrieve weather data. HTTP code: " + String(httpCode));
  }
  http.end();
}

String formatTime() {
  int currentHour = timeClient.getHours();
  int currentMinute = timeClient.getMinutes();
  return (currentHour < 10 ? "0" : "") + String(currentHour) + ":" + (currentMinute < 10 ? "0" : "") + String(currentMinute);
}

String formatDate() {
  unsigned long epochTime = timeClient.getEpochTime();
  struct tm *ptm = gmtime((time_t *)&epochTime);
  int day = ptm->tm_mday;
  int month = ptm->tm_mon + 1;
  int year = ptm->tm_year + 1900;
  return String(day) + "/" + String(month) + "/" + String(year);
}
