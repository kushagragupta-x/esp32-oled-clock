#include <WiFi.h>
#include <time.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include "oled_animation.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDR 0x3C

// Enter your own Wi-Fi details here.
const char* WIFI_SSID = "YOUR_WIFI_SSID";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 5 * 3600 + 30 * 60;
const int daylightOffset_sec = 0;

const uint8_t FRAME_COUNT = sizeof(animationFrames) / sizeof(animationFrames[0]);
const uint32_t FRAME_INTERVAL_MS = 180;

uint32_t lastFrameAt = 0;
uint8_t frameIndex = 0;
int lastDayOfYear = -1;

String cachedWeekday = "";
String cachedDate = "";
bool timeReady = false;

void connectWiFiAndClock() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to Wi-Fi");

  uint32_t start = millis();

  while (WiFi.status() != WL_CONNECTED && millis() - start < 15000) {
    delay(300);
    Serial.print('.');
  }

  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

    struct tm timeinfo;
    Serial.print("Waiting for NTP");

    start = millis();

    while (!getLocalTime(&timeinfo, 500) && millis() - start < 12000) {
      Serial.print('.');
    }

    timeReady = getLocalTime(&timeinfo, 100);
    Serial.println(timeReady ? " synced" : " not synced");
  } else {
    Serial.println("Wi-Fi unavailable. Date will appear after connection.");
  }
}

void updateDateStrings() {
  struct tm timeinfo;

  if (!getLocalTime(&timeinfo, 20)) {
    return;
  }

  timeReady = true;

  if (timeinfo.tm_yday == lastDayOfYear) {
    return;
  }

  lastDayOfYear = timeinfo.tm_yday;

  const char* days[] = {
    "Sunday", "Monday", "Tuesday", "Wednesday",
    "Thursday", "Friday", "Saturday"
  };

  cachedWeekday = days[timeinfo.tm_wday];

  char dateBuffer[11];

  snprintf(
    dateBuffer,
    sizeof(dateBuffer),
    "%02d.%02d.%04d",
    timeinfo.tm_mday,
    timeinfo.tm_mon + 1,
    timeinfo.tm_year + 1900
  );

  cachedDate = dateBuffer;
}

void drawScreen() {
  display.clearDisplay();

  display.drawBitmap(
    0,
    0,
    animationFrames[frameIndex],
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    SSD1306_WHITE
  );

  if (timeReady && cachedWeekday.length()) {
    display.setTextColor(SSD1306_WHITE);
    display.setTextWrap(false);

    display.setFont(&FreeMonoBold9pt7b);
    display.setTextSize(1);
    display.setCursor(0, 37);
    display.print(cachedWeekday);

    display.setFont();
    display.setCursor(0, 49);
    display.print(cachedDate);
  }

  display.display();
}

void setup() {
  Serial.begin(115200);

  Wire.begin(21, 22);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println("SSD1306 not found. Check wiring/address.");

    while (true) {
      delay(100);
    }
  }

  display.clearDisplay();
  display.display();

  connectWiFiAndClock();
  updateDateStrings();
  drawScreen();
}

void loop() {
  if (WiFi.status() == WL_CONNECTED && !timeReady) {
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    updateDateStrings();
  }

  updateDateStrings();

  if (millis() - lastFrameAt >= FRAME_INTERVAL_MS) {
    lastFrameAt = millis();
    frameIndex = (frameIndex + 1) % FRAME_COUNT;
    drawScreen();
  }

  delay(5);
}
