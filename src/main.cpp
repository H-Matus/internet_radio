// Standard Libraries
#include <Arduino.h>
#include <VS1053.h>
#include <HTTPClient.h>
#include <esp_wifi.h>
#include <WiFi.h>

// Custom Libraries


// MP3 Decoder Pins
#define VS1053_CS     5
#define VS1053_DCS    16
#define VS1053_DREQ   4

// Default volume
#define VOLUME 80

VS1053 player(VS1053_CS, VS1053_DCS, VS1053_DREQ);
WiFiClient client;

// WiFi settings example, substitute your own
const char *ssid = "BeBox 3";
const char *password = "snowdrop396";

char *host = "realfm.live24.gr";
char *path = "/realfm";
int httpPort = 80;

// The buffer size 64 seems to be optimal. At 32 and 128 the sound might be brassy.
uint8_t mp3buff[64];

void setup()
{
    Serial.begin(115200);

    // Wait for VS1053 and PAM8403 to power up
    // otherwise the system might not start up correctly
    delay(3000);

    Serial.println("\n\nSimple Radio Node WiFi Radio");

    SPI.begin();

    player.begin();
    player.switchToMp3Mode();
    player.setVolume(100);

    Serial.print("Connecting to SSID ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    Serial.print("connecting to ");
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
    if (!client.connected())
    {
        Serial.println("Reconnecting...");
        if (client.connect(host, httpPort))
        {
            client.print(String("GET ") + path + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
        }
    }

    if (client.available() > 0)
    {
        // The buffer size 64 seems to be optimal. At 32 and 128 the sound might be brassy.
        uint8_t bytesread = client.read(mp3buff, 64);
        player.playChunk(mp3buff, bytesread);
    }
}