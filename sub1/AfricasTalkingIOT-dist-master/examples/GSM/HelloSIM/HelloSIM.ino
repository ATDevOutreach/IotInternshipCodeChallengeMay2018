/*
 Name:		HelloSIM.ino
 Created:	11/24/2017 11:25:49 AM
 Author:	Kennedy Otieno
*/

#define TINY_GSM_MODEM_SIM800
#include <TinyGsmClientSIM800.h>
#include <TinyGsmClient.h>
#include <AfricasTalkingCloud.h>

const char apn[] = "safaricom";
const char user[] = "saf";
const char pass[] = "data";

#define SerialAT Serial1

TinyGsm modem(SerialAT);
TinyGsmClient gsmClient(modem);

void gsmCallBack(char* topic, byte* payload, unsigned int length);

AfricasTalkingCloudClient client(gsmCallBack,gsmClient); // This publishing to Prod...


void setup() {
	Serial.begin(115200);
	delay(100);
	SerialAT.begin(115200);
	delay(5000);

	Serial.println("Initializing Modem");
	modem.restart();
	String modemInfo = modem.getModemInfo();
	Serial.print("Modem: ");
	Serial.println(modemInfo);
	modem.simUnlock("1000");
	Serial.print("Waiting for network...");
	if (!modem.waitForNetwork()) {
		Serial.println(" Failed!");
		while (true);
	}
	Serial.println(" OK");
	Serial.print(F("Connecting to... "));
	Serial.print(apn);
	if (!modem.gprsConnect(apn, user, pass)) {
		Serial.println(F(" Falied"));
		while (true);
	}
	Serial.println(" OK");

}
void loop() {
	if (!client.connected())
	{
		_keepAlive();
	}
	if (!client.connected())
	{
		Serial.println(F("We got disconnected"));
		if(client.connect("SIM800", "<username>", "<password>")) 
		{
			Serial.println(F("Online"));
		}
		else
		{
			Serial.print("Something is wrong: ");
			Serial.println(client.state());
		}
		long now = millis();
		long lastMsg = 0;
		if (now -lastMsg > 5000)
		{
			Serial.println(F("Pinging Server"));
			client.publish("<username>/SIM800/data", "ping");
		}
	}
  
}

void gsmCallBack(char* topic, byte* payload, unsigned int length)
{
	Serial.print("Message arrived [");
	Serial.print(topic);
	Serial.print("]: ");
	Serial.write(payload, length);
	Serial.println();
}

//Reconnect always

void _keepAlive() {

	while (!client.connected()) {
		Serial.print(F("Contacting Server: "));
		Serial.println();
		if (client.connect("SIM800", "<username>", "<password>")) {
			Serial.println("connected");
			client.publish("<username>/SIM800/birth", "SIM800");
		}
		else {
			Serial.print("Failed, rc=");
			Serial.print(client.state());
			Serial.println(" Retrying...");
			delay(6000);
		}
	}
}