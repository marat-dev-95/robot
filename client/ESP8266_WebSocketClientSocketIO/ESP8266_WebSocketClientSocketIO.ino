/*

 * esp8266 simple WebSocket client

 *

 * I use the ws://echo.websocket.org/ echo server

 * When you send a message to this server you receive

 * a response with the same message

 *

 */

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WebSocketsClient.h>

WebSocketsClient webSocket;

#define DEBUG_WEBSOCKETS_PORT     Serial
// Debug Level from 0 to 4
#define _WEBSOCKETS_LOGLEVEL_     4

const char *ssid = "Redmi23";
const char *password = "marat9595";
unsigned long messageInterval = 1000;
bool connected = false;

#define DEBUG_SERIAL Serial


void setup() {
    DEBUG_SERIAL.begin(115200);
    delay(200);
    DEBUG_SERIAL.setDebugOutput(true);
    //DEBUG_SERIAL("HELLO TAK MOZHNOOOO");
    DEBUG_SERIAL.println();
    DEBUG_SERIAL.println();

    

    for(uint8_t t = 4; t > 0; t -- ) {
        DEBUG_SERIAL.printf("[SETUP] BOOT WAIT %d…\n", t);
        DEBUG_SERIAL.flush();
        delay(1000);
    }

    WiFi.begin(ssid, password);

    while ( WiFi.status() != WL_CONNECTED ) {
        delay ( 500 );
        Serial.print ( "." );
    }
    
    DEBUG_SERIAL.print("Local IP: "); 
    DEBUG_SERIAL.println(WiFi.localIP());

    // server address, port and URL
    webSocket.begin("echo.websocket.org", 443, "/");

    Serial.println("kekkich");
    // event handler
    webSocket.onEvent(webSocketEvent);

}

unsigned long lastUpdate = millis();

void loop() {
  if (connected && lastUpdate+messageInterval<millis()){
        DEBUG_SERIAL.println("[WSc] SENT: Simple js client message!!");
        webSocket.sendTXT("Simple Ayush js client message!!");
        lastUpdate = millis();
    }
    webSocket.loop();
    Serial.println("333333333333333333333333333333333333333333333333");
    

}


void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {

    switch(type) {

        case WStype_DISCONNECTED:
           
            DEBUG_SERIAL.printf("[WSc] Disconnected!\n");
            connected = false;
            break;

        case WStype_CONNECTED:
        
            DEBUG_SERIAL.printf("[WSc] Connected to url: %s\n", payload);
            connected = true;
            // send message to server when Connected
            DEBUG_SERIAL.println("[WSc] SENT: Connected");
            webSocket.sendTXT("Connected");
            break;

        case WStype_TEXT:
            DEBUG_SERIAL.printf("[WSc] RESPONSE: %s\n", payload);
            break;

        case WStype_BIN:
            DEBUG_SERIAL.printf("[WSc] get binary length: %u\n", length);
            hexdump(payload, length);
            break;

        case WStype_PING:
            // pong will be send automatically
            DEBUG_SERIAL.printf("[WSc] get ping\n");
            break;

        case WStype_PONG:
            // answer to a ping we send
            DEBUG_SERIAL.printf("[WSc] get pong\n");
            break;

    }

}
