#include <Arduino.h>
#include <WebServer.h>
#include "ServerHandling.h"

void handleOnConnect()
{
    Serial.println("User connected to the server.");
    server.send(200, "text/html", SendHTML());
}

void handleOnNotFound()
{
    server.send(404, "text/plain", "Server is not found.");
}

String SendHTML()
{
    
}