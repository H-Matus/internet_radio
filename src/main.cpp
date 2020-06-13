#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>

struct Button
{
    const uint8_t PIN;
    uint32_t numberKeyPresses;
    bool pressed;
};

Button button1 = {18, 0, false};
const int LedPin = 2;


void IRAM_ATTR isr()
{
    button1.numberKeyPresses += 1;
    button1.pressed = true;
}

void setup()
{
    // Setup
    Serial.begin(9600);
    delay(1000);
    Serial.println("Setup complete");

    // Assigning pins
    pinMode(button1.PIN, INPUT_PULLUP);
    pinMode(LedPin, OUTPUT);

    // ISRs
    attachInterrupt(button1.PIN, isr, FALLING);
}

void loop()
{

    if (button1.pressed)
    {
        Serial.printf("Button 1 has been pressed %u times\n", button1.numberKeyPresses);
        button1.pressed = false;
        digitalWrite(2, HIGH);
        delay(500);
        digitalWrite(2, LOW);
    }

    

    //Detach Interrupt after 1 Minute
    static uint32_t lastMillis = 0;
    if (millis() - lastMillis > 60000)
    {
        lastMillis = millis();
        detachInterrupt(button1.PIN);
        Serial.println("Interrupt Detached!");
    }
}

/*
void setup() 
{
    Serial.begin(9600);
    Serial.println();

    delay(1000);
    
    WiFi.begin("BeBox 3", "snowdrop396");

    Serial.print("Connecting");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println();

    Serial.print("Connected, IP address: ");
    Serial.println(WiFi.localIP());
}
*/