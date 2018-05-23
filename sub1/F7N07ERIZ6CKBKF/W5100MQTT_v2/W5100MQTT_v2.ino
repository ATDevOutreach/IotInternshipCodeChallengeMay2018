/*
          Arduino UNO with W5100 Ethernetshield or  W5100 Ethernet module, used as MQTT client
          It will connect over Wifi to the MQTT broker and controls a digital output (LED, relay)
          and gives the Temperature and Humidity, as well as the state of some switches
          The topics have the format "home/br/sb" for southbound messages and  "home/nb" for northbound messages
          Southbound are messages going to the client, northbound are messages coming from the client
          As the available memory of a UNO  with Ethernetcard is limited, I have kept the topics short
          Also, the payloads  are kept short
          The Northbound topics are
          home/br/nb/temp  for temperature
          home/br/nb/humid  for humidity
          home/br/nb/deur  for a door switch
          home/br/nb/l for  the lightintensity
          home/br/nb/pr  for the status of a PIR sensor
          home/br/nb/ip showing the IP number of the client
          home/br/nb/relay showing the relaystate

          There is only one southbound topic:
          home/br/sb
          The payload here determines the action:
          0 -Switch the relay off
          1-Switch the  relay on
          2-Publish the IP number of the client
          3 Ask for the relaystate

          On Startup, the Client publishes the IP number

*/
#include <Ethernet.h>
#include "PubSubClient.h"
#include "DHT.h"
//kennelijk  geeft update van DHT sensor library boven 1.2.1 een fout
#define CLIENT_ID       "Hal"
//#define TOPIC           "temp"
#define PUBLISH_DELAY   3000
#define PUB_TOPIC "my_username/f/temperature" //Adafruit dashboard
#define PUB_TOPIC_h "my_username/f/humidity" //Adafruit dashboard
#define DHTPIN          3
#define DHTTYPE         DHT11
#define ledPin 13
#define relayPin 8
String ip = "";
bool statusKD = HIGH;
bool statusBD = HIGH;
bool statusGD = HIGH;
bool relaystate = LOW;
bool pir = LOW;
bool startsend = HIGH;
int lichtstatus;
uint8_t mac[6] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x06};

EthernetClient ethClient;
PubSubClient mqttClient;
DHT dht(DHTPIN, DHTTYPE);

long previousMillis;

void setup() {
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(relayPin, OUTPUT);

  // setup serial communication

  Serial.begin(9600);
  while (!Serial) {};
  Serial.println(F("MQTT Arduino Demo"));
  Serial.println();

  // setup ethernet communication using DHCP
  if (Ethernet.begin(mac) == 0) {
    //Serial.println(F("Unable to configure Ethernet using DHCP"));
    for (;;);
  }

  Serial.println(F("Ethernet configured via DHCP"));
  Serial.print("IP address: ");
  Serial.println(Ethernet.localIP());
  Serial.println();
  /*
    Serial.println(Ethernet.localIP()[0]);
    Serial.println(Ethernet.localIP()[1]);
    Serial.println(Ethernet.localIP()[2]);
    Serial.println(Ethernet.localIP()[3]);
  */
  ip = String (Ethernet.localIP()[0]);
  ip = ip + ".";
  ip = ip + String (Ethernet.localIP()[1]);
  ip = ip + ".";
  ip = ip + String (Ethernet.localIP()[2]);
  ip = ip + ".";
  ip = ip + String (Ethernet.localIP()[3]);
  //Serial.println(ip);

  // setup mqtt client
  mqttClient.setClient(ethClient);
  //mqttClient.setServer(mqttServer, 1883);
  // mqttClient.setServer("test.mosquitto.org", 1883);
  //  mqttClient.setServer( "raspberrypi.local",1883);
  //mqttClient.setServer("io.adafruit.com",1883);
  mqttClient.setServer( "192.168.1.102", 1883);
  //Serial.println(F("MQTT client configured"));
  mqttClient.setCallback(callback);
  // setup DHT sensor
  dht.begin();
  Serial.println(F("DHT sensor initialized"));

  Serial.println();
  Serial.println(F("Ready to send data"));
  previousMillis = millis();
  mqttClient.publish("home/br/nb/ip", ip.c_str());
}

void loop() {

  statusBD = digitalRead(4);// FrontdoorSwitch
  statusGD = digitalRead(5);// Garagedoor Switch
  statusKD = (digitalRead(6));//LivingRoom Switch

  lichtstatus = analogRead(A0);//Reads an LDR
  pir = digitalRead(7);//Reads a PIR sensor
  relaystate = digitalRead(relayPin);// Reads the state of a relay

  // it's time to send new data?
  if (millis() - previousMillis > PUBLISH_DELAY) {
    sendData();
    previousMillis = millis();

  }

  mqttClient.loop();
}

void sendData() {

  char msgBuffer[20];
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println("oC");
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.println("%");
  Serial.print("Relay is: ");
  Serial.println((relaystate == LOW) ? "OPEN" : "CLOSED");
  if (mqttClient.connect(CLIENT_ID)) {
    mqttClient.publish("home/br/nb/temp", dtostrf(t, 6, 2, msgBuffer));
    mqttClient.publish("home/br/nb/humid", dtostrf(h, 6, 2, msgBuffer));
    mqttClient.publish("home/br/nb/deur", (statusBD == HIGH) ? "OPEN" : "CLOSED");
    mqttClient.publish("home/br/nb/garage", (statusGD == HIGH) ? "OPEN" : "DICHT");
    mqttClient.publish("home/br/nb/bel", (statusKD == HIGH) ? "OPEN" : "CLOSED");
    mqttClient.publish("home/br/nb/l", dtostrf(lichtstatus, 4, 0, msgBuffer));
    mqttClient.publish("home/br/nb/p", (pir == HIGH) ? "OPEN" : "CLOSED");
    mqttClient.publish("home/br/nb/relay", (relaystate == LOW) ? "OPEN" : "CLOSED");
    mqttClient.subscribe("home/br/sb");
    if (startsend) {
     // mqttClient.publish("home/br/nb/relay", (relaystate == LOW) ? "OPEN" : "CLOSED");
      mqttClient.publish("home/br/nb/ip", ip.c_str());
      startsend = LOW;
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  char msgBuffer[20];
  // I am only using one ascii character as command, so do not need to take an entire word as payload
  // However, if you want to send full word commands, uncomment the next line and use for string comparison
   //payload[length] = '\0';            // terminate string with '0'
  //String strPayload = String((char*)payload);  // convert to string
  // Serial.println(strPayload); //can use this if using longer southbound topics
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");//MQTT_BROKER
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  Serial.println(payload[0]);

  // Examine only the first character of the message
  if (payload[0] == 49)             // Message "1" in ASCII (turn output ON)
  {
    digitalWrite(LED_BUILTIN, HIGH);    //
    digitalWrite(relayPin, HIGH);
  } else if (payload[0] == 48)      // Message "0" in ASCII (turn output OFF)
  {
    digitalWrite(relayPin, LOW);     //
    digitalWrite(LED_BUILTIN, LOW);
  } else if (payload[0] == 50)
  {
    mqttClient.publish("home/br/nb/ip", ip.c_str());// publish IP nr
  } else {
    Serial.println("Unknown value");
    mqttClient.publish("home/br/nb", "Syntax Error");
  }

}
