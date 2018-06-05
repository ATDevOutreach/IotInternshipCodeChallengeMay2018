#include <Arduino.h>
#include "WiFi101.h"
#include <SPI.h>
#include "AfricasTalkingCloud.h"

const char username[] = "thor";
const char password[] = "avengers";
const char device_id[] = "WIFI101";
const char sensor_topic[] = "thor/megaatmega2560/sensor";
const char command_topic[] = "thor/megaatmega2560/commands";
const char birth_topic[] = "thor/megaatmega2560/birth";
char message[50];
const byte led_pin = 5;
const byte ldr_pin = A0;
uint8_t led_state = 0;
int status = WL_IDLE_STATUS;

WiFiClient client101;
void setup(void);
void loop(void);
void connectToAP(void);
void wifiCallBack(char topic[], byte* payload, unsigned int length);
void reconnect(void);

AfricasTalkingCloudClient client(wifiCallBack, client101);

const char ssid[] = "MySecureWiFiSSID";
const char pass[] = "MySuperSecretWiFiPassword";

void setup(void)
{
    WiFi.setPins(10,3,5);
    while(!Serial);
    Serial.begin(9600);
    connectToAP();   
}

void loop(void)
{
    
    if (!client.connected()) {
        reconnect();
    }
    /*Omit if necessary*/
    while(!client.loop()) //IF?
    {
        Serial.print(F("Device disconnected.Status:"));
        Serial.print(client.state());        
        if (client.connect(device_id, username, password)) {
            Serial.println(F("Connected to the server!"));
        }
        else {
            Serial.print(F("An error occured.Status:"));
            Serial.println(client.state());
        }
    }
    long sensor_val = analogRead(ldr_pin);
    snprintf(message, 50, "%ld", sensor_val);
    client.publish(sensor_topic,message,1);
    delay(5000);
}

void wifiCallBack(char topic[], byte* payload, unsigned int length)
{
    String command = "";
    // int comp = strcmp(topic,command_topic);
    String received_topic = (String)topic;
    
    for(unsigned int i = 0; i < length; i++)
    {
        command += (char)payload[i];
    }
    if ((received_topic == command_topic) && (command == "ON")) {
        digitalWrite(led_pin, (!led_state));
    }
    else if((received_topic == command_topic) && (command == "OFF"))
    {
        digitalWrite(led_pin, led_state);
    }
}

void connectToAP()
{
    Serial.println(F("Setting up connection...."));
    if (WiFi.status() == WL_NO_SHIELD) {
        Serial.println(F("WiFi shield not present"));
        while (true);
    }
    while (status != WL_CONNECTED) {
        Serial.print(F("Attempting to connect to WPA SSID: "));
        Serial.println(ssid);
        status = WiFi.begin(ssid, pass);
        delay(10000);
    }
    Serial.print("You're connected to the network");
}

void reconnect(void)
{
    while (!client.connected()) 
    {
        connectToAP();
        Serial.print(F("Contacting Server: "));
        Serial.println();
    if (!client.connect(device_id, username, password))
    {
        Serial.print(F("Failed to Connect to Broker"));
        Serial.println(client.state());
        }
    Serial.println(F("Re/Connected"));
    client.publish(birth_topic, "WIFI101-MODULE-birth");
    client.subscribe(command_topic, 0);
    }
}