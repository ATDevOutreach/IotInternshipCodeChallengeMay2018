/*
 Name:		HelloEdison.ino
 Created:	10/16/2017 15:21:27
 Author:	Kennedy Otieno
*/
#include <WiFi.h>
#include <SPI.h>
#include "<AfricasTalkingCloud.h>"

long duration = 0;
char payloadData[50];
char ssid[] = "net";
char password[] = "pass";
int value = 0;
long lastMsg = 0;
char msg[50];
int status = WL_IDLE_STATUS;
WiFiClient edisonClient;
AfricasTalkingCloudClient client(edisonClient);
// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(115200);
	while (!Serial)
	{
		;
	}
	initiateConnection();
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
		if (client.connect("Edison", "username", "password"))
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
		client.publish("username/Edison/sensors", msg);
	}
}

void initiateConnection()
{
	//Check if WiFi Shield Exists
	if (WiFi.status() == WL_NO_SHIELD)
	{
		Serial.println(F("WiFi shield not Present"));
		while (true)
		{
			;
		}
	}
	String fW = WiFi.firmwareVersion();
	if (fW != "1.1.0")
	{
		Serial.println(F("Check your Firmware"));
	}
	//If shield is found and we're not connected already
	while (WiFi.status() != WL_CONNECTED)
	{
		Serial.print(F("Attempting to connect to : "));
		Serial.println(ssid);

		//Connect to a WPA Network
		WiFi.begin(ssid, password);
		delay(10000);
	}
	//You're connected
	Serial.println(F("You are connnected"));
	getConnectionDetails();
}

void getConnectionDetails()
{
	//Get Your IP
	IPAddress localIP = WiFi.localIP();
	Serial.print(F("Your IP: "));
	Serial.println(localIP);
	Serial.println(localIP);
	Serial.println();

	//Get your Mac Address
	byte mac[6];
	WiFi.macAddress(mac);
	Serial.print("MAC address: ");
	Serial.print(mac[5], HEX);
	Serial.print(":");
	Serial.print(mac[4], HEX);
	Serial.print(":");
	Serial.print(mac[3], HEX);
	Serial.print(":");
	Serial.print(mac[2], HEX);
	Serial.print(":");
	Serial.print(mac[1], HEX);
	Serial.print(":");
	Serial.println(mac[0], HEX);
	Serial.println();
}

void _keepAlive()
{
	while (!client.connected()) {
		Serial.print(F("Contacting Server: "));
		Serial.println();
		if (client.connect("Edison", "username", "kapua-password")) {
			Serial.println("connected");
			client.publish("username/Edison/hello", "Edison");
			client.subscribe("username/Edison/data");
		}
		else {
			Serial.print("Failed, rc=");
			Serial.print(client.state());
			Serial.println(" Retrying...");
			delay(6000);
		}
	}
}
