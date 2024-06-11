#include <ESP8266WiFi.h>
#include <WebSocketClient.h>

const char* ssid = "Redmi23";
const char* password = "marat9595";

WebSocketClient ws(true);

void setup() {
  Serial.begin(9600);
  delay(100);

  // Подключение к Wi-Fi сети
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
	if (!ws.isConnected()) {
    Serial.println("here one");
		Serial.println(ws.connect("echo.websocket.org", "/", 443));
	} else {
    Serial.println("here two");
		ws.send("hello");

		String msg;
		if (ws.getMessage(msg)) {
			Serial.println(msg);
		}
	}
	delay(500);
}