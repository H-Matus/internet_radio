// Standard Libraries
#include <Arduino.h>
#include <VS1053.h>
#include <HTTPClient.h>
#include <esp_wifi.h>
#include <WiFi.h>
#include <WebServer.h>

// Custom Libraries
#include "ServerHandling.h"

// MP3 Decoder Pins
#define VS1053_CS     5
#define VS1053_DCS    16
#define VS1053_DREQ   4

// Default volume
#define VOLUME 70

// Define MP3 decoder and WiFi client
VS1053 player(VS1053_CS, VS1053_DCS, VS1053_DREQ);
WiFiClient client;

// Define WiFi name and password of the local network
const char *ssid = "BeBox 3";
const char *password = "snowdrop396";

// Define host, path and HTTP Port of the main radio station
char *host = "82.135.234.195"; // realfm.live24.gr
char *path = "/kelyje_vilnius.mp3"; // /realfm
int httpPort = 8000;

// Define buffer size. 64 seems to be optimal. At 32 and 128 the sound might be brassy.
uint8_t mp3buff[64];

void setup()
{
    // Initialise serial bus
    Serial.begin(115200);

    // Wait for VS1053 and PAM8403 to power up
    // otherwise the system might not start up correctly
    delay(3000);
    Serial.println("\n\nESP32 Wifi Radio");

    // Initialise SPI bus
    SPI.begin();

    // Initialise MP3 decoder
    player.begin();
    player.switchToMp3Mode();
    player.setVolume(VOLUME);

    // Connect to the local network
    Serial.print("Connecting to local network\nSSID: ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    // Initialise ESP32 HTTP Server
    server.on("/", handleOnConnect);
    server.onNotFound(handleOnNotFound);

    Serial.print("Connecting to ");
    Serial.println(host);

    if (!client.connect(host, httpPort))
    {
        Serial.println("Connection failed");
        return;
    }

    Serial.print("Requesting stream: ");
    Serial.println(path);

    client.print(String("GET ") + path + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
}

void loop()
{
    // Reconnect, if lost connection
    if (!client.connected())
    {
        Serial.println("Reconnecting...");
        if (client.connect(host, httpPort))
        {
            client.print(String("GET ") + path + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
        }
    }

    // Play the incoming bitstream, if there are availables bits
    if (client.available() > 0)
    {
        // The buffer size 64 seems to be optimal. At 32 and 128 the sound might be brassy.
        uint8_t bytesread = client.read(mp3buff, 64);
        player.playChunk(mp3buff, bytesread);
    }
}