/*
 Name:		ESP8266Hello.ino
 Created:	10/6/2017 15:19:26
 Author:	Kennedy Otieno
*/

// In this example remember to change the baord type to ESP8266 Generic Module

#include <ESP8266WiFi.h>
#include <AfricasTalkingCloud.h>

WiFiClient esp8266;
const char ssid[] = "AfricasTalking Kura GW";
const char pass[] = "AfricasTalking";
char msg[50];
long value = 0;
long lastMsg = 0;
void subMsgCallBack(char* topic, byte* payload, unsigned int length);
AfricasTalkingCloudClient client(subMsgCallBack, esp8266);
void setup() {
	Serial.begin(115200);
	while (!Serial)
	{
		;
	}
	connectToNetwork();
}

// the loop function runs over and over again until power down or reset
void loop() {
	if (!client.connected())
	{
		_keepAlive();
	}
	if (!client.loop())
	{
		Serial.println(F("Disconnected..."));
		if (client.connect("ESP8266", "<username>", "<password>"))
		{
			Serial.println(F("Reconnected..."));
		}
		else {
			Serial.println(F("Failed Reconnection..."));
		}
	}
	long now = millis();
	if (now - lastMsg > 5000) {
		lastMsg = now;
		++value;
		snprintf(msg, 75, "%ld", value);
		Serial.print("Payload: ");
		Serial.println(msg);
		client.publish("<username>/ESP8266/sensors", msg);
	}
}
void connectToNetwork()
{
	WiFi.mode(WIFI_STA);
	Serial.print(F("Attempting to Connect to: "));
	Serial.println(ssid);
	WiFi.begin(ssid, pass);
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(F("."));
	}
	Serial.println(F(""));
	Serial.println(F("WiFi connected"));
	Serial.println(F("IP address: "));
	Serial.println(WiFi.localIP());
}
void subMsgCallBack(char * topic, byte * payload, unsigned int length)
{
	Serial.print(F("Message arrived ["));
	Serial.print(topic);
	Serial.print(F("] "));
	for (unsigned int i = 0; i < length; i++) {
		Serial.print((char)payload[i]);
	}
	Serial.println();
}

void _keepAlive()
{
	while (!client.connected()) {
		Serial.print(F("Contacting Server: "));
		Serial.println();
		if (client.connect("ESP8266", "<username>", "<password>")) {
			Serial.println("connected");
			client.publish("<username>/ESP8266/hello", "ESP8266");
			client.subscribe("<username>/ESP8266/data");
		}
		else {
			Serial.print("Failed, rc=");
			Serial.print(client.state());
			Serial.println(" Retrying...");
			delay(6000);
		}
	}
}
