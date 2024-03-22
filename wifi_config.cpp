#include <wifi_config.h>

const int BUFFER_SIZE = 1024;
const int GET = 0;
const int OPTION = 2;
const int POST = 1;
const int DATA_SYMBOL = 100;

#if __has_include(<EEPROM.h>)
void saveConfig(WiFiConfig *config)
{
    EEPROM.write(0, DATA_SYMBOL);
    Serial.print("WRITING DATA TO EEPROM: ");
    Serial.println(DATA_SYMBOL);

    int eepromIdx = 1;
    int bufferIdx = 0;

    while (true)
    {
        EEPROM.write(eepromIdx++, config->ssid[bufferIdx]);
        Serial.println(config->ssid[bufferIdx]);
        if (config->ssid[bufferIdx] == '\0')
        {
            break;
        }
        bufferIdx++;
    }
    bufferIdx = 0;
    while (true)
    {
        EEPROM.write(eepromIdx++, config->password[bufferIdx]);
        if (config->password[bufferIdx] == '\0')
        {
            break;
        }
        bufferIdx++;
    }

    if (config->ip)
    {
        IPAddress ip = *config->ip;
        EEPROM.write(eepromIdx++, DATA_SYMBOL);
        EEPROM.write(eepromIdx++, ip[0]);
        EEPROM.write(eepromIdx++, ip[1]);
        EEPROM.write(eepromIdx++, ip[2]);
        EEPROM.write(eepromIdx++, ip[3]);
    }
    else
    {

        EEPROM.write(eepromIdx++, 0);
    }
}

void freeRequest(Request *req)
{
}

WiFiConfig *loadConfig()
{
    if (EEPROM.read(0) != DATA_SYMBOL)
    {
        return NULL;
    }
    struct WiFiConfig *config = (struct WiFiConfig *)malloc(sizeof(struct WiFiConfig));
    char buffer[64];
    int eepromIdx = 1;
    int bufferIdx = 0;
    while (true)
    {
        buffer[bufferIdx] = EEPROM.read(eepromIdx++);
        if (buffer[bufferIdx] == '\0')
        {
            break;
        }
        bufferIdx++;
    }
    config->ssid = (char *)malloc(sizeof(char) * (strlen(buffer) + 1));
    strcpy(config->ssid, buffer);

    bufferIdx = 0;
    while (true)
    {
        buffer[bufferIdx] = EEPROM.read(eepromIdx++);
        if (buffer[bufferIdx] == '\0')
        {
            break;
        }
        bufferIdx++;
    }

    config->password = (char *)malloc(sizeof(char) * (strlen(buffer) + 1));
    strcpy(config->password, buffer);

    if (EEPROM.read(eepromIdx++) != DATA_SYMBOL)
    {
        return config;
    }

    IPAddress ip(
        EEPROM.read(eepromIdx),
        EEPROM.read(eepromIdx + 1),
        EEPROM.read(eepromIdx + 2),
        EEPROM.read(eepromIdx + 3));

    config->ip = (IPAddress *)malloc(sizeof(IPAddress));
    *config->ip = ip;

    return config;
}
#endif

void printWiFiStatus()
{
    if (WiFi.SSID() == NULL)
    {
        Serial.println("[WC] NOT CONNECTED TO WIFI");
        return;
    }
    Serial.print("[WC] CONNECTED TO SSID: ");
    Serial.println(WiFi.SSID());

    IPAddress ip = WiFi.localIP();
    Serial.print("[WC] IP Address: ");
    Serial.println(ip);
    byte bssid[6];
    WiFi.BSSID(bssid);

    long rssi = WiFi.RSSI();
    Serial.print("[WC] Signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");
}

int connectToWiFi(WiFiConfig *config)
{
    if (config->ip)
    {
        Serial.print("SETTING IP TO: ");
        Serial.println(config->ip->toString());
        WiFi.config(*config->ip);
    }
    int connectionAttempts = 2;
    int wifiStatus = WL_IDLE_STATUS;
    while (wifiStatus != WL_CONNECTED)
    {
        if (connectionAttempts-- < 0)
        {
            return NULL;
        }
        Serial.print("[WC] Attempting to connect to SSID: ");
        Serial.println(config->ssid);
        wifiStatus = WiFi.begin(config->ssid, config->password);

        delay(2000);
    }

    return wifiStatus;
}

bool checkWiFi()
{
    if (WiFi.status() == WL_NO_MODULE)
    {
        Serial.println("[WC] Communication with WiFi module failed!");
        while (true)
            ;
    }

    if (WiFi.firmwareVersion() < WIFI_FIRMWARE_LATEST_VERSION)
    {
        Serial.println("[WC] Please upgrade the firmware");
    }
}

bool isPrefix(char *buffer, char *pre)
{
    return strncmp(buffer, pre, strlen(pre)) == 0;
}

void setMethodAndURL(Request *request, char *buffer, int length)
{
    int offset = 5;
    if (isPrefix(buffer, "GET"))
    {
        request->method = GET;
    }
    else if (isPrefix(buffer, "POST"))
    {
        request->method = POST;
        offset = 6;
    }
    else if (isPrefix(buffer, "OPTION"))
    {
        request->method = OPTION;
        offset = 7;
    }
    strcpy(request->path, &buffer[offset]);
}

Request *handleConnection(WiFiClient *client)
{
    Serial.print("[HTTP] CLIENT CONNECTED:");
    Serial.println(client->connected());
    char buffer[BUFFER_SIZE];
    int i = 0;
    struct Request *request = (struct Request *)malloc(sizeof(struct Request));
    request->ip = NULL;

    while (client->connected())
    {
        delayMicroseconds(10); // SOMEONE SAID I NEED THIS
        if (client->available())
        {
            char c = client->read();
            if (c == '\n')
            {
                buffer[i] = '\0';
                Serial.println(buffer);

                if (i == 0)
                {
                    return request;
                }
                if (
                    isPrefix(buffer, "GET") || isPrefix(buffer, "POST") || isPrefix(buffer, "OPTION"))
                {
                    setMethodAndURL(request, buffer, i);
                    Serial.print("[WC] GOT A `");
                    Serial.print(request->method);
                    Serial.print("` REQUEST: `");
                    Serial.print(request->path);
                    Serial.println('`');
                }

                if (request->method == OPTION)
                {
                    return request;
                }
                if (request->method == GET)
                {
                    if (strcmp(request->path, "hi") == 0)
                    {

                        return request;
                    }
                }
                if (isPrefix(buffer, "ssid"))
                {
                    request->ssid = (char *)malloc(sizeof(char) * (strlen(&buffer[6]) + 1));
                    strcpy(request->ssid, &buffer[6]);
                }
                else if (isPrefix(buffer, "password"))
                {
                    request->password = (char *)malloc(sizeof(char) * (strlen(&buffer[10]) + 1));
                    strcpy(request->password, &buffer[10]);
                }
                else if (isPrefix(buffer, "ip"))
                {
                    request->ip = (IPAddress *)malloc(sizeof(IPAddress));
                    request->ip->fromString(&buffer[4]);
                    Serial.println(request->ip->toString());
                }
                i = 0;
            }
            else if (i < BUFFER_SIZE)
            {
                if (c != '\r')
                {
                    buffer[i++] = c;
                }
            }
            else
            {
                return request;
            }
        }
    }
    return request;
}

void sendHeaders(WiFiClient *client)
{
    client->println("HTTP/1.1 200 OK");
    client->println("Content-Type: text/html");
    client->println("Connection: close");
    client->println("Access-Control-Allow-Origin: *");
    client->println("Access-Control-Allow-Methods: *");
    client->println("Access-Control-Allow-Headers: *");
    client->println("Access-Control-Allow-Credentials: true");
}

void sendResponse(WiFiClient *client, char *buffer)
{
    sendHeaders(client);
    client->println();
    client->println(buffer);
    client->println();
    client->println();
}

void sendOptionResponse(WiFiClient *client)
{
    sendHeaders(client);
    client->println();
    client->println();
}

void sendConfigResponse(WiFiClient *client, WiFiConfig *config)
{
    sendHeaders(client);

    client->println();
    client->print("password: ");
    client->println(config->password);
    client->print("ssid: ");
    client->println(config->ssid);
    if (config->ip != NULL)
    {
        client->print("ip: ");
        client->println(config->ip->toString());
    }
    client->println();
    client->println();
}

Request *loopHTTP(WiFiServer *wifiServer, WiFiConfig *config)
{
    WiFiClient client = wifiServer->available();
    if (client)
    {
        Request *request = handleConnection(&client);

        if (request->method == OPTION)
        {
            sendOptionResponse(&client);
        }
        else if (request->method == GET)
        {
            if (isPrefix(request->path, "health"))
            {
                sendResponse(&client, ";)");
            }
            else
            {
                sendConfigResponse(&client, config);
            }
            delay(1);
        }
        else if (request->method == POST)
        {
            sendResponse(&client, "OK!");
            delay(1);
        }
        client.stop();
        return request;
    }

    return NULL;
}

void startAndRunAccessPoint(WiFiConfig *config)
{
    WiFiServer wifiServer(80);
    startAndRunAccessPoint(config, &wifiServer);
    while (connectToWiFi(config) != WL_CONNECTED)
    {
        Serial.println("[WC] Failed to connect to wifi restarting Access Point");
        startAndRunAccessPoint(config, &wifiServer);
    }
}

void startAndRunAccessPoint(WiFiConfig *config, WiFiServer *wifiServer)
{
    Serial.println("[WC] creating access point: `Arduino WiFi Config AP`");
    WiFi.config(IPAddress(192, 48, 56, 2));
    int wifiStatus = WiFi.beginAP("Arduino WiFi Config AP");
    printWiFiStatus();
    if (wifiStatus != WL_AP_LISTENING)
    {
        Serial.println("[WC] Creating access point failed");
        while (true)
            ;
    }

    Serial.println("[WC] WAITING FOR WIFI TO START");
    delay(5000);
    wifiServer->begin();
    Serial.println("[WC] WAITING FOR A CONNECTION");

    while (true)
    {
        Request *request = loopHTTP(wifiServer, config);
        if (request == NULL)
            continue;
        if (isPrefix(request->path, "run"))
        {
            Serial.println("[WC] Shutting Down AP");
            wifiServer->end();
            WiFi.end();
            freeRequest(request);
            return;
        }
        else if (isPrefix(request->path, "set"))
        {
            Serial.println("[WC] UPDATING CONFIG");
            if (request->ssid)
            {
                config->ssid = (char *)malloc(sizeof(char) * (strlen(request->ssid) + 1));
                strcpy(config->ssid, request->ssid);
                free(request->ssid);
            }
            if (request->password)
            {
                config->password = (char *)malloc(sizeof(char) * (strlen(request->password) + 1));
                strcpy(config->password, request->password);
                free(request->password);
            }
            config->ip = request->ip;
            Serial.println(config->ssid);
            Serial.println(config->password);
            Serial.println(config->ip->toString());
            Serial.println("[WC] CONFIG UPDATED");
        }
        free(request);
    }
}