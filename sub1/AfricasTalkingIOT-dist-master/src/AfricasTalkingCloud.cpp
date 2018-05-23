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
*/

#include "AfricasTalkingCloud.h"
#include <Arduino.h>
//#include <ESP8266WiFi/src/WiFiClientSecure.h>

#ifdef ESP8266
	#define INIT_FINGERPRINT() this->fingerprint = nullptr;
#else
	#define INIT_FINGERPRINT()
#endif

uint16_t AfricasTalkingCloudClient::read_packet(uint8_t* length_length)
{
	uint16_t len = 0;
	if (!read_byte(buffer_, &len))
	{
		return 0;
	}
	boolean is_publish = (buffer_[0] & 0xF0) == MQTTPUBLISH;
	uint32_t multiplier = 1;
	uint16_t length = 0;
	uint8_t digit = 0;
	uint16_t skip = 0;
	uint8_t start = 0;

	do
	{
		if (!read_byte(&digit))
		{
			return 0;
		}
		buffer_[len++] = digit;
		length += (digit & 127) * multiplier;
		multiplier <<=7;
	}
	while ((digit & 128) != 0);
	*length_length = len - 1;

	if (is_publish)
	{
		// Read in topic length to calculate bytes to skip over for Stream writing
		if (!read_byte(buffer_, &len))
		{
			return 0;
		}
		if (!read_byte(buffer_, &len))
		{
			return 0;
		}
		skip = (buffer_[*length_length + 1] << 8) + buffer_[*length_length + 2];
		start = 2;
		if (buffer_[0] & MQTTQOS1)
		{
			// skip message id
			skip += 2;
		}
	}

	for (auto i = start; i < length; i++)
	{
		if (!read_byte(&digit))
		{
			return 0;
		}
		if (this->stream_)
		{
			if (is_publish && len - *length_length - 2 > skip)
			{
				this->stream_->write(digit);
			}
		}
		if (len < MQTT_MAX_PACKET_SIZE)
		{
			buffer_[len] = digit;
		}
		len++;
	}

	if (!this->stream_ && len > MQTT_MAX_PACKET_SIZE)
	{
		len = 0; // Ignore incoming packet.
	}

	return len;
}

boolean AfricasTalkingCloudClient::read_byte(uint8_t* result)
{
	uint32_t previous_millis = millis();
	while (!available())
	{
		uint32_t current_millis = millis();
		if (current_millis - previous_millis >= (static_cast<int32_t>(MQTT_SOCKET_TIMEOUT) * 1000))
		{
			return false;
		}
	}
	*result = client_->read();
	available_ -= 1;
	return true;
}

boolean AfricasTalkingCloudClient::read_byte(uint8_t* result, uint16_t* index)
{
	auto current_index = *index;
	auto write_address = &(result[current_index]);
	if (read_byte(write_address))
	{
		*index = current_index + 1;
		return true;
	}
	return false;
}

boolean AfricasTalkingCloudClient::write(uint8_t header, uint8_t* buf, uint16_t length)
{
	uint8_t lenBuf[4];
	uint8_t llen = 0;
	uint8_t pos = 0;
	uint16_t len = length;
	do
	{
		auto digit = len & 127;
		len >>= 7;
		if (len > 0)
		{
			digit |= 0x80;
		}
		lenBuf[pos++] = digit;
		llen++;
	}
	while (len > 0);

	buf[4 - llen] = header;
	for (auto i = 0; i < llen; i++)
	{
		buf[5 - llen + i] = lenBuf[i];
	}

#ifdef MQTT_MAX_TRANSFER_SIZE
	uint8_t* writeBuf = buf + (4 - llen);
	uint16_t bytesRemaining = length + 1 + llen;  //Match the length type
	uint8_t bytesToWrite;
	boolean result = true;
	while ((bytesRemaining > 0) && result) {
		bytesToWrite = (bytesRemaining > MQTT_MAX_TRANSFER_SIZE) ? MQTT_MAX_TRANSFER_SIZE : bytesRemaining;
		rc = _client->write(writeBuf, bytesToWrite);
		result = (rc == bytesToWrite);
		bytesRemaining -= rc;
		writeBuf += rc;
	}
	return result;
#else
	uint16_t rc = client_->write(buf + (4 - llen), length + 1 + llen);
	last_out_activity_ = millis();
	return (rc == 1 + llen + length);
#endif
}

uint16_t AfricasTalkingCloudClient::write_string(const char* string, uint8_t* buf, uint16_t pos)
{
	const char* idp = string;
	uint16_t i = 0;
	pos += 2;
	while (*idp)
	{
		buf[pos++] = *idp++;
		i++;
	}
	buf[pos - i - 2] = (i >> 8);
	buf[pos - i - 1] = (i & 0xFF);
	return pos;
}

int AfricasTalkingCloudClient::available()
{
	if (available_ == 0)
	{
		available_ = client_->available();
	}
	return available_;
}

AfricasTalkingCloudClient::AfricasTalkingCloudClient()
{
	this->state_ = MQTT_DISCONNECTED;
	this->client_ = nullptr;
	this->stream_ = nullptr;
	set_callback(nullptr);
	this->available_ = 0;
	INIT_FINGERPRINT()
}

AfricasTalkingCloudClient::AfricasTalkingCloudClient(Client& client)
{
	this->state_ = MQTT_DISCONNECTED;
	set_client(client);
	const char* domain = production_url_;
	const uint16_t port = production_port_;
	set_server(domain, port);
	this->stream_ = nullptr;
	this->available_ = 0;
	INIT_FINGERPRINT()
}

AfricasTalkingCloudClient::AfricasTalkingCloudClient(MQTT_CALLBACK_SIGNATURE, Client& client)
{
	this->state_ = MQTT_DISCONNECTED;
	const char* domain = production_url_;
	const uint16_t port = production_port_;
	set_server(domain, port);
	set_callback(callback);
	set_client(client);
	this->available_ = 0;
	INIT_FINGERPRINT()
}

AfricasTalkingCloudClient::AfricasTalkingCloudClient(boolean environment, MQTT_CALLBACK_SIGNATURE, Client& client, Stream& stream)
{
	const char* domain = sandbox_url_;
	const uint16_t port = sandbox_port_;
	this->state_ = MQTT_DISCONNECTED;
	if (environment)
	{
		set_server(domain, port);
	}
	set_client(client);
	set_callback(callback);
	set_stream(stream);
	this->available_ = 0;
	INIT_FINGERPRINT()
}

AfricasTalkingCloudClient::AfricasTalkingCloudClient(boolean environment, MQTT_CALLBACK_SIGNATURE, Client& client)
{
	const char* domain = sandbox_url_;
	const uint16_t port = sandbox_port_;
	this->state_ = MQTT_DISCONNECTED;
	if (environment)
	{
		set_server(domain, port);
	}
	set_client(client);
	set_callback(callback);
	this->stream_ = nullptr;
	this->available_ = 0;
	INIT_FINGERPRINT()
}

AfricasTalkingCloudClient::AfricasTalkingCloudClient(boolean environment, Client& client)
{
	const char* domain = sandbox_url_;
	const uint16_t port = sandbox_port_;
	this->state_ = MQTT_DISCONNECTED;
	if (environment)
	{
		set_server(domain, port);
	}
	set_client(client);
	this->stream_ = nullptr;
	this->available_ = 0;
	INIT_FINGERPRINT()
}

AfricasTalkingCloudClient::AfricasTalkingCloudClient(MQTT_CALLBACK_SIGNATURE, Client& client, Stream& stream)
{
	const char* domain = production_url_;
	const uint16_t port = production_port_;
	this->state_ = MQTT_DISCONNECTED;
	set_server(domain, port);
	set_callback(callback);
	set_client(client);
	set_stream(stream);
	this->available_ = 0;
	INIT_FINGERPRINT()
}

boolean AfricasTalkingCloudClient::loop()
{
	if (connected())
	{
		do
		{
		auto t = millis();
		if ((t - last_in_activity_ > MQTT_KEEPALIVE * 1000UL) || (t - last_out_activity_ > MQTT_KEEPALIVE * 1000UL))
		{
			if (ping_outstanding_)
			{
				this->state_ = MQTT_CONNECTION_TIMEOUT;
				client_->stop();
				return false;
			}
			buffer_[0] = MQTTPINGREQ;
			buffer_[1] = 0;
			client_->write(buffer_, 2);
			last_out_activity_ = t;
			last_in_activity_ = t;
			ping_outstanding_ = true;
		}
		while (client_->available())
		{
			uint8_t llen;
			uint16_t len = read_packet(&llen);
			uint16_t msg_id = 0;
			uint8_t* payload;
			if (len > 0)
			{
				last_in_activity_ = t;
				uint8_t type = buffer_[0] & 0xF0;
				if (type == MQTTPUBLISH)
				{
					if (callback)
					{
						uint16_t tl = (buffer_[llen + 1] << 8) + buffer_[llen + 2];
						memmove(buffer_ + llen + 2 ,buffer_ + llen + 3, tl);
						buffer_[llen +2 + tl] = 0;
						char *topic = reinterpret_cast<char*>(buffer_) + llen +2;
						if ((buffer_[0] & 0x06) == MQTTQOS1)
						{
							msg_id = (buffer_[llen + 3 + tl] << 8) + buffer_[llen + 3 + tl + 1];
							payload = buffer_ + llen + 3 + tl + 2;
							callback(topic, payload, len - llen - 3 - tl - 2);
							buffer_[0] = MQTTPUBACK;
							buffer_[1] = 2;
							buffer_[2] = (msg_id >> 8);
							buffer_[3] = (msg_id & 0xFF);
							client_->write(buffer_, 4);
							last_out_activity_ = t;
						}
						else
						{
							payload = buffer_ + llen + 3 + tl;
							callback(topic, payload, len - llen - 3 - tl);
						}
					}

				}
				else if (type == MQTTPUBREC)
				{
					buffer_[0] = MQTTPUBREL | 2;
					client_->write(buffer_, 4);
				}
				else if (type == MQTTPINGREQ)
				{
					buffer_[0] = MQTTPINGRESP;
					buffer_[1] = 0;
					client_->write(buffer_, 2);
				}
				else if (type == MQTTPINGRESP)
				{
					ping_outstanding_ = false;
				}
			}
		}
	}while (available_ > 0); // Potential spurious code
		return true;
	}
	return false;
}

boolean AfricasTalkingCloudClient::connected()
{
	boolean rc;
	if (client_ == nullptr)
	{
		rc = false;
	}
	else
	{
		rc = static_cast<int>(client_->connected());
		if (!rc)
		{
			if (this->state_ == MQTT_CONNECTED)
			{
				this->state_ = MQTT_CONNECTION_LOST;
				client_->flush();
				client_->stop();
			}
		}
	}
	return rc;
}

int AfricasTalkingCloudClient::state()
{
	return this->state_;
}

AfricasTalkingCloudClient& AfricasTalkingCloudClient::set_server(const char* domain, const uint16_t port)
{
	this->domain_ = domain;
	this->port_ = port;
	return *this;
}

AfricasTalkingCloudClient& AfricasTalkingCloudClient::set_callback(MQTT_CALLBACK_SIGNATURE)
{
	this->callback = callback;
	return *this;
}

AfricasTalkingCloudClient& AfricasTalkingCloudClient::set_client(Client& client)
{
	this->client_ = &client;
	return *this;
}

AfricasTalkingCloudClient& AfricasTalkingCloudClient::set_stream(Stream& stream)
{
	this->stream_ = &stream;
	return *this;
}


#ifdef ESP8266
AfricasTalkingCloudClient::AfricasTalkingCloudClient(WiFiClientSecure & client, const char * fingerprint)
{
	const char* domain = "sungura1-anagin-ke-host.africastalking.com";
	const uint16_t port = 18883;
	this->state_ = MQTT_DISCONNECTED;
	set_client(client);
	this->stream_ = nullptr;
	this->available_ = 0;
	this->fingerprint = fingerprint;
	set_server(domain, port);
}

AfricasTalkingCloudClient::AfricasTalkingCloudClient(WiFiClientSecure & client, const char * fingerprint, boolean environment)
{
	const char* domain = "sungura1-anagin-ke-host.africastalking.com";
	const uint16_t port = 18883;
	this->state_ = MQTT_DISCONNECTED;
	if (environment)
	{
		set_server(domain, port);
	}
	set_client(client);
	this->stream_ = nullptr;
	set_client(client);
	this->fingerprint = fingerprint;
}

AfricasTalkingCloudClient::AfricasTalkingCloudClient(WiFiClientSecure & client, const char * fingerprint, boolean environment, MQTT_CALLBACK_SIGNATURE)
{
	const char* domain = "sungura1-anagin-ke-host.africastalking.com";
	const uint16_t port = 18883;
	this->state_ = MQTT_DISCONNECTED;
	if (environment)
	{
		set_server(domain, port);
	}
	set_client(client);
	this->stream_ = nullptr;
	set_client(client);
	this->fingerprint = fingerprint;
	set_callback(callback);
}

AfricasTalkingCloudClient::AfricasTalkingCloudClient(WiFiClientSecure & client, const char * fingerprint, boolean environment, MQTT_CALLBACK_SIGNATURE, Stream& stream)
{
	const char* domain = "sungura1-anagin-ke-host.africastalking.com";
	const uint16_t port = 18883;
	this->state_ = MQTT_DISCONNECTED;
	if (environment)
	{
		set_server(domain, port);
	}
	set_client(client);
	this->stream_ = nullptr;
	set_client(client);
	this->fingerprint = fingerprint;
	set_callback(callback);
	set_stream(stream);
}

AfricasTalkingCloudClient::AfricasTalkingCloudClient(WiFiClientSecure & client, const char * fingerprint, MQTT_CALLBACK_SIGNATURE)
{
	const char* domain = "sungura1-anagin-ke-host.africastalking.com";
	const uint16_t port = 18883;
	this->state_ = MQTT_DISCONNECTED;
	set_server(domain, port);
	set_client(client);
	this->stream_ = nullptr;
	set_client(client);
	this->fingerprint = fingerprint;
	set_callback(callback);
}

AfricasTalkingCloudClient::AfricasTalkingCloudClient(WiFiClientSecure & client, const char * fingerprint, MQTT_CALLBACK_SIGNATURE, Stream& stream)
{
	const char* domain = "sungura1-anagin-ke-host.africastalking.com";
	const uint16_t port = 18883;
	this->state_ = MQTT_DISCONNECTED;
	set_server(domain, port);
	set_client(client);
	this->stream_ = nullptr;
	set_client(client);
	this->fingerprint = fingerprint;
	set_callback(callback);
	set_stream(stream);
}
#endif

boolean AfricasTalkingCloudClient::connect(const char* device_id, const char* username, const char* password)
{
	return connect(device_id, username, password, nullptr, 0, false, nullptr);
}

boolean AfricasTalkingCloudClient::connect(const char* device_id, const char* username, const char* password, const char* will_topic, const uint8_t will_qos, const boolean will_retain, const char* will_message)
{
	if (!connected())
	{
		int result = 0;
		if (domain_ != nullptr)
		{
			result = client_->connect(this->domain_, this->port_);
		}

#ifdef ESP8266
		if(fingerprint != nullptr)
		{
			if(domain_ != nullptr)
			{
				if (!static_cast<WiFiClientSecure*>(client_)->verify(fingerprint,domain_))
				{
					state_ = MQTT_TLS_BAD_SERVER_CREDENTIALS;
					return false;
				}
			}
			
		}
#endif

		if (result > 0)
		{
			next_msg_id_ = 1;
			// Leave room in the buffer for header and variable length field
			uint16_t length = 5;

#if MQTT_VERSION == MQTT_VERSION_3_1
			uint8_t d[9] = { 0x00,0x06,'M','Q','I','s','d','p', MQTT_VERSION };
#define MQTT_HEADER_VERSION_LENGTH 9
#elif MQTT_VERSION == MQTT_VERSION_3_1_1
			uint8_t d[7] = {0x00,0x04,'M','Q','T','T',MQTT_VERSION};
			//http://docs.oasis-open.org/mqtt/mqtt/v3.1.1/csd02/mqtt-v3.1.1-csd02.html#_Toc385349226
			/*
			Here's an MQTT Variable Header Structure
			----------------------------------------------------------------------
			|				|	Description		|	7|	6|	5|	4|	3|	2|	1|	0|
			----------------------------------------------------------------------
			|Protocol Name														 |
			----------------------------------------------------------------------
			|byte 1			|Length MSB(0==0x00)|	0|	0|	0|	0|	0|	0|	0|	0|
			----------------------------------------------------------------------
			|byte 2			|Length LSB(4==0x04)| 	0|	0|	0|	0|	0|	1|	0|	0|
			----------------------------------------------------------------------
			|byte 3			| 'M'				|	0|	1|	0|	0|	1|	1|	0|	1|   This is decimal number 77 equivalent to Letter M
			----------------------------------------------------------------------
			|byte 4			| 'Q'				|	0|	1|	0|	1|	0|	0|	0|	1|   This is decimal number 81 equivalent to Letter Q
			----------------------------------------------------------------------
			|byte 5			| 'T'				|	0|	1|	0|	1|	0|	1|	0|	0|	 This is decimal number 84 equivalent to Letter T
			----------------------------------------------------------------------
			|byte 6			| 'T'				|	0|	1|	0|	1|	0|	1|	0|	0|	 This is decimal number 84 equivalent to Letter T
			----------------------------------------------------------------------

			Here's an MQTT Protocol Level Structure
			----------------------------------------------------------------------
			|				|	Description		|	7|	6|	5|	4|	3|	2|	1|	0|
			----------------------------------------------------------------------
			|Protocol Name														 |
			----------------------------------------------------------------------
			|byte 7			|Level 4			|	0|	0|	0|	0|	0|	1|	0|	0|  Recall #define MQTT_VERSION_3_1_1    4  from header file
			----------------------------------------------------------------------
			*/
#define MQTT_HEADER_VERSION_LENGTH 7
#endif
			for (unsigned int j = 0; j < MQTT_HEADER_VERSION_LENGTH; j++)
			{
				buffer_[length++] = d[j];
			}

			uint8_t v;
			if (will_topic)
			{
				v = 0x06 | (will_qos << 3) | (will_retain << 5);
			}
			else
			{
				v = 0x02;
			}

			if (username != nullptr)
			{
				v = v | 0x80;

				if (password != nullptr)
				{
					v = v | (0x80 >> 1);
					//CONNECT FLAGS ARE IN BYTE 8 http://docs.oasis-open.org/mqtt/mqtt/v3.1.1/csd02/mqtt-v3.1.1-csd02.html#_Toc385349230
				}
			}

			buffer_[length++] = v;

			buffer_[length++] = ((MQTT_KEEPALIVE) >> 8);
			buffer_[length++] = ((MQTT_KEEPALIVE) & 0xFF);
			length = write_string(device_id, buffer_, length);
			if (will_topic)
			{
				length = write_string(will_topic, buffer_, length);
				length = write_string(will_message, buffer_, length);
			}

			if (username != nullptr)
			{
				length = write_string(username, buffer_, length);
				if (password != nullptr)
				{
					length = write_string(password, buffer_, length);
				}
			}

			write(MQTTCONNECT, buffer_, length - 5);

			last_in_activity_ = last_out_activity_ = millis();

			while (!available())
			{
				unsigned long t = millis();
				if (t - last_in_activity_ >= (static_cast<int32_t>(MQTT_SOCKET_TIMEOUT) * 1000UL))
				{
					state_ = MQTT_CONNECTION_TIMEOUT;
					client_->stop();
					return false;
				}
			}
			uint8_t llen;
			uint16_t len = read_packet(&llen);

			if (len == 4)
			{
				if (buffer_[3] == 0)
				{
					last_in_activity_ = millis();
					ping_outstanding_ = false;
					state_ = MQTT_CONNECTED;
					return true;
				}
				state_ = buffer_[3];
			}
			client_->stop();
		}
		else
		{
			state_ = MQTT_CONNECT_FAILED;
		}
		return false;
	}
	return true;
}

void AfricasTalkingCloudClient::disconnect()
{
	buffer_[0] = MQTTDISCONNECT;
	buffer_[1] = 0;
	client_->write(buffer_, 2);
	state_ = MQTT_DISCONNECTED;
	client_->stop();
	last_in_activity_ = last_out_activity_ = millis();
}

boolean AfricasTalkingCloudClient::publish(const char* topic, const char* payload)
{
	return publish(topic, reinterpret_cast<const uint8_t*>(payload), strlen(payload), 0,false);
}

boolean AfricasTalkingCloudClient::publish(const char* topic, const char* payload, uint8_t qos)
{
	return publish(topic, reinterpret_cast<const uint8_t*>(payload), strlen(payload), qos, false);
}

boolean AfricasTalkingCloudClient::publish(const char* topic, const uint8_t* payload, unsigned int plength)
{
	return publish(topic, payload, plength, 0, false);
}

boolean AfricasTalkingCloudClient::publish(const char * topic, const char * payload, uint8_t qos, boolean retained)
{
	return publish(topic, reinterpret_cast<const uint8_t*>(payload), strlen(payload), qos, retained);
}

boolean AfricasTalkingCloudClient::publish(const char* topic, const uint8_t* payload, unsigned int plength, boolean retained)
{
	return publish(topic, payload, plength, 0, retained);
}

boolean AfricasTalkingCloudClient::publish(const char* topic, const uint8_t* payload, unsigned int plength, uint8_t qos, boolean retained)
{
	if (connected())
	{
		if (MQTT_MAX_PACKET_SIZE < 5 + 2 + strlen(topic) + plength + (qos ? 2 : 0))
		{
			// Too long
			return false;
		}
		// Leave room in the buffer for header and variable length field
		uint16_t length = 5;
		length = write_string(topic, buffer_, length);
		if (qos)
		{
			next_msg_id_++;
			if (next_msg_id_ == 0)
			{
				next_msg_id_ = 1;
			}
			buffer_[length++] = (next_msg_id_ >> 8);
			buffer_[length++] = (next_msg_id_ & 0xFF);
		}
		for (uint16_t i = 0; i < plength; i++)
		{
			buffer_[length++] = payload[i];
		}
		uint8_t header = MQTTPUBLISH;
		if (retained)
		{
			header |= 1;
		}
		header |= qos << 1;

		return write(header, buffer_, length - 5);
	}
	return false;
}

boolean AfricasTalkingCloudClient::publish_progmem(const char* topic, const uint8_t* payload, unsigned int plength)
{
	return publish_progmem(topic, payload, plength, 0, false);
}

boolean AfricasTalkingCloudClient::publish_progmem(const char* topic, const uint8_t* payload, unsigned int plength, boolean retained)
{
	return publish_progmem(topic, payload, plength, 0, retained);
}

boolean AfricasTalkingCloudClient::publish_progmem(const char* topic, const uint8_t* payload, unsigned int plength, uint8_t qos, boolean retained)
{
	uint8_t llen = 0;
	unsigned int rc = 0;
	unsigned int pos = 0;

	if (!connected())
	{
		return false;
	}

	uint16_t tlen = strlen(topic);

	uint8_t header = MQTTPUBLISH;
	if (retained)
	{
		header |= 1;
	}
	header |= qos << 1;
	buffer_[pos++] = header;
	unsigned int len = plength + 2 + tlen;
	do
	{
		uint8_t digit = len &127;
		len >>= 7;
		if (len > 0)
		{
			digit |= 0x80;
		}
		buffer_[pos++] = digit;
		llen++;
	} while (len > 0);

	pos = write_string(topic, buffer_, pos);

	rc += client_->write(buffer_, pos);

	for (unsigned int i = 0; i < plength; i++)
	{
		rc += client_->write((char)pgm_read_byte_near(payload + i));
	}

	last_out_activity_ = millis();

	return rc == tlen + 3 + llen + plength;
}

boolean AfricasTalkingCloudClient::subscribe(const char* topic)
{
	return subscribe(topic, 0);
}

boolean AfricasTalkingCloudClient::subscribe(const char* topic, const uint8_t qos)
{
	//We can not subscribe to QOS2 Topics
	if (qos > 1)
	{
		return false;
	}
	if (MQTT_MAX_PACKET_SIZE < 9 + strlen(topic))
	{
		// Too long
		return false;
	}
	if (connected())
	{
		// Leave room in the buffer for header and variable length field
		uint16_t length = 5;
		next_msg_id_++;
		if (next_msg_id_ == 0)
		{
			next_msg_id_ = 1;
		}
		buffer_[length++] = (next_msg_id_ >> 8);
		buffer_[length++] = (next_msg_id_ & 0xFF);
		length = write_string(const_cast<char*>(topic), buffer_, length);
		buffer_[length++] = qos;
		return write(MQTTSUBSCRIBE | MQTTQOS1, buffer_, length - 5);
	}
	return false;
}

boolean AfricasTalkingCloudClient::unsubscribe(const char* topic)
{
	if (MQTT_MAX_PACKET_SIZE < 9 + strlen(topic))
	{
		// Too long
		return false;
	}
	if (connected())
	{
		uint16_t length = 5;
		next_msg_id_++;
		if (next_msg_id_ == 0)
		{
			next_msg_id_ = 1;
		}
		buffer_[length++] = (next_msg_id_ >> 8);
		buffer_[length++] = (next_msg_id_ & 0xFF);
		length = write_string(topic, buffer_, length);
		return write(MQTTUNSUBSCRIBE | MQTTQOS1, buffer_, length - 5);
	}
	return false;
}
