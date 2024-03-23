// UNO R4 WiFi
// #include <WiFiS3.h>
// #include <EEPROM.h>

// Nano IOT
#include <WiFiNINA.h>
#include <FlashAsEEPROM.h>

#include <wifi_config.h>

bool ledValue = false;

void setup()
{
  Serial.begin(115200);
  while (!Serial)
  {
    ;
  }

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  Serial.println("[WC] WIFI CONFIG RUNNING");

  WiFiConfig newConfig;
  startAndRunAccessPoint(&newConfig);
  printWiFiStatus();
}

void loop()
{
  delay(1000);
  ledValue = !ledValue;
  digitalWrite(LED_BUILTIN, ledValue ? HIGH : LOW);
}
