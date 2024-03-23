#ifndef QUICKPING_WIFI_H

#if __has_include(<WiFiS3.h>)
#include <WiFiS3.h>
#else
#include <WiFiNINA.h>
#endif

#if __has_include(<EEPROM.h>)
#include <EEPROM.h>
#else
#include <FlashAsEEPROM.h>
#endif
struct WiFiConfig
{
    char *ssid;
    char *password;
    IPAddress *ip;
};

struct Request
{
    int method;
    char path[33];
    char *ssid;
    char *password;
    IPAddress *ip;
};

void saveConfig(WiFiConfig *config);
WiFiConfig *loadConfig();
int connectToWiFi(WiFiConfig *config);
void printWiFiStatus();
void startAndRunAccessPoint(WiFiConfig *config, WiFiServer *wifiServer);
void startAndRunAccessPoint(WiFiConfig *config);

#define QUICKPING_WIFI_H
#endif