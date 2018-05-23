/**
* Name:	AfricasTalkingCloud.cpp
* Created:	10/4/2017 18:58:52
* Author:	Kennedy Otieno
* Editor:	http://www.visualmicro.com
* Initial Contributors :
*					  1. Nick O'Leary
*					  2. git:@matsujirushi
*					  3. git:@Jonatan Olofsson
*					  4. git:@constantin
*					  5. git:@Jorge Cisneros
*					  6. git:@Nguyễn Hồng Quân
*					  7. git:@Johan Philippe
*					  8. git:@Diego Guerrero et.al.
*					  9. https://github.com/vshymanskyy for the wonderful client library for SIM && XBee modules
*					  10. Callback arbitrator code added by Jonathan Olofsson
*/

#ifndef AFRICAS_TALKING_CLOUD_H
#define AFRICAS_TALKING_CLOUD_H

#include "Arduino.h"
#include "IPAddress.h"
#include "Client.h"
#include "Stream.h"

#define MQTT_VERSION_3_1      3
#define MQTT_VERSION_3_1_1    4
// #define MQTT_VERSION 5_0   5

//We support version 3.1.1 by default
#ifndef MQTT_VERSION
#define MQTT_VERSION MQTT_VERSION_3_1_1
#endif // !MQTT_VERSION

//Maximum Packet Size:This will be overriden by buffer size
//MQTT imposes a maximum payload size of 268435455 bytes.Minimum is 128
//http://docs.oasis-open.org/mqtt/mqtt/v3.1.1/os/mqtt-v3.1.1-os.html#_Toc385349213
//http://docs.oasis-open.org/mqtt/mqtt/v3.1.1/os/mqtt-v3.1.1-os.html#_Toc385349213
#ifndef MQTT_MAX_PACKET_SIZE
#define MQTT_MAX_PACKET_SIZE 512 // Allow for larger packet sizes
#endif // !MQTT_MAX_PACKET_SIZE

//MQTT_KEEP_ALIVE
//Set the number of seconds after which the bridge should send a ping if no other traffic has occurred.
//Defaults to 60. A minimum value of 5 seconds is allowed.
#ifndef MQTT_KEEPALIVE
#define MQTT_KEEPALIVE 15
#endif // !MQTT_KEEPALIVE

//MQTT_SOCKET_TIMEOUT
#ifndef MQTT_SOCKET_TIMEOUT
#define MQTT_SOCKET_TIMEOUT 15
#endif // !MQTT_SOCKET_TIMEOUT

//? MAXIMUM TRANSFER SIZE

//Wait for Acknowledgement of QOS1 messages
#ifndef QOS1_WAIT_TIME
#define QOS1_WAIT_TIME 100
#endif // !QOS1_WAIT_TIME


// http://docs.oasis-open.org/mqtt/mqtt/v3.1.1/os/mqtt-v3.1.1-os.html#_Toc398718035
#define MQTT_TLS_BAD_SERVER_CREDENTIALS -5
#define MQTT_CONNECTION_TIMEOUT         -4
#define MQTT_CONNECTION_LOST		    -3
#define MQTT_CONNECT_FAILED             -2
#define MQTT_DISCONNECTED				-1
#define MQTT_CONNECTED					 0 // We are connected
#define MQTT_CONNECT_BAD_PROTOCOL		 1 // Server does not support the MQTT version requested by the client
#define MQTT_CONNECT_BAD_CLIENT_ID		 2 // The Client ID is correct but not allowed by the server
#define MQTT_CONNECT_UNAVAILABLE		 3 // Network connection made but server is unavailable
#define MQTT_CONNECT_BAD_CREDENTIALS	 4 // User credentials are malformed
#define MQTT_CONNECT_UNAUTHORIZED		 5 // The client is not authorized to conect

//MQTT Control Packets that contain a Payload
//http://docs.oasis-open.org/mqtt/mqtt/v3.1.1/os/mqtt-v3.1.1-os.html#_Toc398718026
#define MQTTCONNECT     1 << 4  // Client request to connect to Server (Required)
#define MQTTCONNACK     2 << 4  // Connect Acknowledgment (No payload)
#define MQTTPUBLISH     3 << 4  // Publish message (optional payload)
#define MQTTPUBACK      4 << 4  // Publish Acknowledgment (No Payload)
#define MQTTPUBREC      5 << 4  // Publish Received (assured delivery part 1)
#define MQTTPUBREL      6 << 4  // Publish Release (assured delivery part 2)
#define MQTTPUBCOMP     7 << 4  // Publish Complete (assured delivery part 3)
#define MQTTSUBSCRIBE   8 << 4  // Client Subscribe request
#define MQTTSUBACK      9 << 4  // Subscribe Acknowledgment
#define MQTTUNSUBSCRIBE 10 << 4 // Client Unsubscribe request
#define MQTTUNSUBACK    11 << 4 // Unsubscribe Acknowledgment
#define MQTTPINGREQ     12 << 4 // PING Request
#define MQTTPINGRESP    13 << 4 // PING Response
#define MQTTDISCONNECT  14 << 4 // Client is Disconnecting
#define MQTTReserved    15 << 4 // Reserved

//Quality of service and protocol flows
//http://docs.oasis-open.org/mqtt/mqtt/v3.1.1/os/mqtt-v3.1.1-os.html#_Toc398718099
#define MQTTQOS0        (0 << 1)
#define MQTTQOS1        (1 << 1)
#define MQTTQOS2        (2 << 1)

#ifdef ESP8266
#include <functional>
#define MQTT_CALLBACK_SIGNATURE std::function<void(char*, uint8_t*, unsigned int)> callback
#else
#define MQTT_CALLBACK_SIGNATURE void (*callback)(char*, uint8_t*, unsigned int)
#endif

// Note: 
// Sandbox broker url	  → broker.sandbox.africastalking.com
//						  → secure.sandbox.africastalking.com

// Similarly for prod	  → broker.africastalking.com
//						  → secure.broker.africastalking.com

// Default broker URL - Prod
#define PRODUCTION_URL   "sungura1-angani-ke-host.africastalking.com"
#define PRODUCTION_PORT  10883

// Default broker URL - Sandbox
#define SANDBOX_URL      "sungura1-angani-ke-host.africastalking.com"
#define SANDBOX_PORT     10883

// Default Secure URL - Prod
#define SSL_PRODUCTION_URL "sungura1-angani-ke-host.africastalking.com"
#define SSL_PRODUCTION_PORT 18883

// Defines Secure Sandbox port [todo]
#define SSL_SANDBOX_URL   "sungura1-anagin-ke-host.africastalking.com"
#define SSL_SANDBOX_PORT 18883

#ifdef ESP8266
#include "WiFiClientSecure.h"
#endif

class AfricasTalkingCloudClient
{
private:
	Client* client_;
	uint8_t buffer_[MQTT_MAX_PACKET_SIZE]{};
	uint16_t next_msg_id_{};
	unsigned long last_out_activity_{};
	unsigned long last_in_activity_{};
	bool ping_outstanding_{};
	MQTT_CALLBACK_SIGNATURE{};
	uint16_t read_packet(uint8_t*);
	boolean read_byte(uint8_t* result);
	boolean read_byte(uint8_t* result, uint16_t* index);
	boolean write(uint8_t header, uint8_t* buf, uint16_t length);
	uint16_t write_string(const char* string, uint8_t* buf, uint16_t pos);
	IPAddress ip_;
	const char* domain_{};
	uint16_t port_{};
	const char* production_url_ = PRODUCTION_URL;
	uint16_t production_port_ = PRODUCTION_PORT;
	const char* sandbox_url_ = SANDBOX_URL;
	uint16_t sandbox_port_ = SANDBOX_PORT;

	Stream* stream_;
	int state_;
	int available();
	int available_;

 #ifdef ESP8266
	const char* fingerprint;
 #endif

public:
	AfricasTalkingCloudClient();
	explicit AfricasTalkingCloudClient(Client& client);
	AfricasTalkingCloudClient(MQTT_CALLBACK_SIGNATURE, Client& client);
	AfricasTalkingCloudClient(boolean, MQTT_CALLBACK_SIGNATURE, Client& client, Stream&);
	AfricasTalkingCloudClient(boolean, MQTT_CALLBACK_SIGNATURE, Client& client); 
	AfricasTalkingCloudClient(boolean, Client& client); 
	AfricasTalkingCloudClient(MQTT_CALLBACK_SIGNATURE, Client& client, Stream&); 
	AfricasTalkingCloudClient& set_server(const char* domain, uint16_t port);
	AfricasTalkingCloudClient& set_callback(MQTT_CALLBACK_SIGNATURE);
	AfricasTalkingCloudClient& set_client(Client& client);
	AfricasTalkingCloudClient& set_stream(Stream& stream);

    #ifdef ESP8266
	AfricasTalkingCloudClient(WiFiClientSecure& client, const char* fingerprint);
	AfricasTalkingCloudClient(WiFiClientSecure& client, const char* fingerprint, MQTT_CALLBACK_SIGNATURE);
	AfricasTalkingCloudClient(WiFiClientSecure& client, const char* fingerprint, boolean, MQTT_CALLBACK_SIGNATURE, Stream&);
	AfricasTalkingCloudClient(WiFiClientSecure& client, const char* fingerprint, boolean, MQTT_CALLBACK_SIGNATURE);
	AfricasTalkingCloudClient(WiFiClientSecure& client, const char* fingerprint, boolean);
	AfricasTalkingCloudClient(WiFiClientSecure& client, const char* fingerprint, MQTT_CALLBACK_SIGNATURE, Stream&);
	#endif
	
	boolean connect(const char* device_id, const char* username, const char* password);
	boolean connect(const char* device_id, const char* username, const char* password, const char* will_topic, uint8_t will_qos, boolean will_retain, const char* will_message);
	void disconnect();
	boolean publish(const char* topic, const char* payload);
	boolean publish(const char* topic, const char* payload, uint8_t qos);
	boolean publish(const char* topic, const char* payload, uint8_t qos, boolean retained);
	boolean publish(const char* topic, const uint8_t* payload, unsigned int plength);
	boolean publish(const char* topic, const uint8_t* payload, unsigned int plength, boolean retained);
	boolean publish(const char* topic, const uint8_t* payload, unsigned int plength, uint8_t qos, boolean retained);
    boolean publish_progmem(const char* topic, const uint8_t* payload, unsigned int plength);
	boolean publish_progmem(const char* topic, const uint8_t* payload, unsigned int plength, boolean retained);
	boolean publish_progmem(const char* topic, const uint8_t* payload, unsigned int plength, uint8_t qos, boolean retained);
	boolean subscribe(const char* topic);
	boolean subscribe(const char* topic, uint8_t qos);
	boolean unsubscribe(const char* topic);
	boolean loop();
	boolean connected();
	int state();
};
#endif

