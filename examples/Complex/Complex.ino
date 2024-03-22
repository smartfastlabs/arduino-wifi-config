#include <WiFiS3.h>
#include <EEPROM.h>
#include <wifi_config.h>

bool ledValue = false;

bool promptForWiFiConfig()
{
  unsigned long startTime = millis();
  Serial.println("[WC] Send any value to configure wifi...");

  while (millis() - startTime < 5000)
  {
    if (Serial.available() > 0)
    {
      return true;
    }
  }
  return false;
}

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

  WiFiConfig *wifiConfig = loadConfig();
  if (wifiConfig == NULL || promptForWiFiConfig())
  {
    WiFiConfig newConfig;
    if (wifiConfig)
      newConfig = *wifiConfig;
    startAndRunAccessPoint(&newConfig);
    saveConfig(&newConfig);
  }
  else
  {
    Serial.println("[WC] Config Found, Connecting to WiFi...");
    if (connectToWiFi(wifiConfig) != WL_CONNECTED)
    {
      startAndRunAccessPoint(wifiConfig);
      saveConfig(wifiConfig);
    }
  }

  printWiFiStatus();
}

void loop()
{
  delay(1000);
  ledValue = !ledValue;
  digitalWrite(LED_BUILTIN, ledValue ? HIGH : LOW);
}
