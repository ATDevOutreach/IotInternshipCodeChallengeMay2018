/*
 Name:		CC3000Publish.ino
 Created:	10/6/2017 14:30:42
 Author:	Kennedy Otieno
*/
#include <AfricasTalkingCloud.h>
#include <SPI.h>
#include <Adafruit_CC3000.h>
#define ADAFRUIT_CC3000_IRQ   3  // MUST be an interrupt pin!
#define ADAFRUIT_CC3000_VBAT  5
#define ADAFRUIT_CC3000_CS    10

Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT, SPI_CLOCK_DIVIDER);
Adafruit_CC3000_Client cc3000client = Adafruit_CC3000_Client();
boolean sandbox = true;
AfricasTalkingCloudClient client(sandbox, cc3000client);
//Network Settings
char ssid[] =  "WiFi";
char pass[] =  "Pass";
#define WLAN_SEC WLAN_SEC_WPA2

char msg[50];
long value = 0;
long lastMsg = 0;

void setup() {
	Serial.begin(9600);
	while (!Serial)
	{
		;
	}
	cc3000Connect();
}

void loop() {
	if (!client.connected())
	{
		_keepAlive();
	}
	if (!client.loop())
	{
		Serial.println(F("Disconnected...Reconnecting."));
		if (client.connect("CC3000", "<username>", "<password>"))
		{
			Serial.println(F("Reconnected..."));
		}
		else
		{
			Serial.println(F("Reconnection failed."));
		}
	}
	long now = millis();
	if (now - lastMsg > 5000) {
		lastMsg = now;
		++value;
		snprintf(msg, 75, "%ld", value);
		Serial.print("Payload: ");
		Serial.println(msg);
		client.publish("<username>/CC3000/sensors", msg);
	}
}
void cc3000Connect()
{
	//If we encountered an error Initializing CC3000
	Serial.println(F("Attempting CC300 Init"));
	if (!cc3000.begin()) {
		Serial.println("An error occured Initializing CC3000");
		for (;;);
	}
	//Meanwhile, Delete Old Connection Pofiles and Attempt to Connect to our network
	Serial.println(F("Attempting to delete old profiles"));

	if (!cc3000.deleteProfiles()) {
		Serial.println("We encountered an error Deleting Profiles");
		while (1);
	}
	Serial.print("Attempting to connect to ");
	Serial.println(ssid);

	if (!cc3000.connectToAP(ssid, pass, WLAN_SEC)) {
		Serial.println("Could not Connect");
		while (1);
	}

	//Connected
	Serial.println("Connected!");

	Serial.println(F("Request DHCP"));
	while (!cc3000.checkDHCP()) {
		delay(100);
	}
	//Display Connection Details
	displayConnectionDetails();

}
bool displayConnectionDetails(void)
{
	uint32_t ipAddress, netmask, gateway, dhcpserv, dnsserv;

	if (!cc3000.getIPAddress(&ipAddress, &netmask, &gateway, &dhcpserv, &dnsserv))
	{
		Serial.println(F("Unable to retrieve the IP Address!\r\n"));
		return false;
	}
	else
	{
		Serial.print(F("\nIP Addr: ")); cc3000.printIPdotsRev(ipAddress);
		Serial.print(F("\nNetmask: ")); cc3000.printIPdotsRev(netmask);
		Serial.print(F("\nGateway: ")); cc3000.printIPdotsRev(gateway);
		Serial.print(F("\nDHCPsrv: ")); cc3000.printIPdotsRev(dhcpserv);
		Serial.print(F("\nDNSserv: ")); cc3000.printIPdotsRev(dnsserv);
		Serial.println();
		return true;
	}
}
void _keepAlive()
{
	while (!client.connected()) {
		Serial.print(F("Contacting Server"));
		Serial.println();
		if (client.connect("CC3000", "<username>", "<password>")) {
			Serial.println("connected");
			client.publish("<username>/CC3000/hello", "CC3000");
			client.subscribe("<username>/CC3000/data");
		}
		else {
			Serial.print("Failed, rc=");
			Serial.print(client.state());
			Serial.println();
			Serial.println(" Retrying...");
			delay(6000);
		}
	}
}