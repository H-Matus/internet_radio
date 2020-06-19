#include <WebServer.h>

#ifndef _SERVER_HANDLING_H_
#define _SERVER_HANDLING_H_

// Define web server with HTTP port
WebServer server(80);

// Define functions
void handleOnConnect();
void handleOnNotFound();

#endif