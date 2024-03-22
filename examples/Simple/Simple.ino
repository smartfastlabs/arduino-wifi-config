#include <WiFiS3.h>
#include <EEPROM.h>
#include <wifi_config.h>

WiFiConfig wifiConfig;
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

  startAndRunAccessPoint(&wifiConfig);
  int wifiStatus = connectToWiFi(&wifiConfig);
  while (wifiStatus != WL_CONNECTED)
  {
    Serial.println("Failed to connect to wifi restarting Access Point");
    startAndRunAccessPoint(&wifiConfig);
    wifiStatus = connectToWiFi(&wifiConfig);
  }

  printWiFiStatus();
}

void loop()
{
  delay(1000);
  ledValue = !ledValue;
  digitalWrite(LED_BUILTIN, ledValue ? HIGH : LOW);
  Serial.println("HERE");
}
