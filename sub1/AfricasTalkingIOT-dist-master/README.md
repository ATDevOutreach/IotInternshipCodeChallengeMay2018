# Africa's Talking Cloud MQTT Library for Arduino  [![Build Status](https://travis-ci.org/TheBeachMaster/AfricasTalkingIOT.svg?branch=master)](https://travis-ci.org/TheBeachMaster/AfricasTalkingIOT) [![CircleCI](https://circleci.com/gh/TheBeachMaster/AfricasTalkingIOT/tree/master.svg?style=svg)](https://circleci.com/gh/TheBeachMaster/AfricasTalkingIOT/tree/master)



### Features:

------

+ SSL support
+ QoS 0,1,2 publishing
+ QoS 0,1 subscribing
+ Support for wide range of modules
+ Lightweight

## Documentations and Samples 
Have a look at Code samples [here](https://thebeachmaster.github.io/AfricasTalkingIOT/#samples)

## Installation 
> TODO 

## Usage   

> Once installed include the following header file in your Arduino project 

 ```c++  
#include <AfricasTalkingCloud.h>  
 ```
> NOTES ON GETTING STARTED:

> + Setting up for the *sandbox*  environment
>
>   ```c++
>   boolean sandbox = true;
>   ```
>
>   Note that this argument is not required when using a live environment.As illustrated in these examples
>
>   ```c++
>   /** This is a sample extracted from the Wi-Fi101 / ATWIC1500 samples
>   *   We create a WiFi client instance
>   *   We then setup our environment to sandbox(for testing)
>   *   We create a callback function - to be defined later in the code
>   *	Then pass these arguments to our Africa's Talking cloud-client instance
>   */
>   WiFiClient _client101;
>   boolean sandbox = true;
>   void callback(char* topic, byte* payload, unsigned int length);
>   AfricasTalkingCloudClient client(sandbox, callback, _client101); 
>   ```

> - For a live environment(default) we could have this set up as so 
>
>   ```c++
>   WiFiClient _client101;
>   void callback(char* topic, byte* payload, unsigned int length);
>   AfricasTalkingCloudClient client(callback, _client101);
>   ```
>
>   For publish-only routines/operations:
>
>   ```c++
>   // Sandbox sample 
>   WiFiClient sandbox101;
>   boolean sandbox = true;
>   AfricasTalkingCloudClient client(sandbox, sandbox101); 
>   ... // The rest of the code
>   ```
>
>   ```c++
>   // Live server
>   WiFiClient prod101;
>   AfricasTalkingCloudClient client(prod101);
>   ... // Other code
>   ```

## Important 

> If you're building for Intel Edison,this library may fail to compile.To make it work set `-std=c++11`  flag as part of the compilation flags.Refer to [Issue #14](https://github.com/TheBeachMaster/AfricasTalkingIOT/issues/14)  

## Callbacks 

> This function is called when new messages arrive at the client.
 ```c++ 
void callback(char[] topic, byte* payload, unsigned int length); 
 ```
 > Then define it as follows , for example
 ```c++  
 void callback(char [] topic, byte* payload, unsigned int length)
{
	Serial.println(topic); // Prints the topic
	for (int i = 0; i < length; i++) {
		Serial.print((char)payload[i]); // Prints the contents of the topic
	}
	Serial.println();
}
 ```
## Quality of Service (Notes)

The QoS level used to deliver an Application Message outbound to the Client could differ from that of the inbound Application Message.

- #### QoS 0 : At most once delivery

The message is delivered according to the capabilities of the underlying network. No response is sent by the receiver and no retry is performed by the sender. The message arrives at the receiver either once or not at all.

- #### QoS 1 : At least once delivery

This quality of service ensures that the message arrives at the receiver at least once. A QoS 1 PUBLISH Packet has a Packet Identifier in its variable header and is acknowledged by a PUBACK Packet. 

- #### QoS 2 : Exactly once delivery 

This is the highest quality of service, for use when neither loss nor duplication of messages are acceptable. There is an increased overhead associated with this quality of service.

## Publishing data ( QoS0, QoS1, QoS2) 

#### QoS 0 publishing:

```c++
char msg[] = "data";
 client.publish("<username/topic/Topic>", msg); 
```

#### QoS 1 publishing:

```c++
char msg[] = "data";
client.publish("<username/topic/Topic>", msg, 1); 
```

#### QoS 2 publishing:

```c++
char msg[] = "data";
client.publish("<username/topic/Topic>", msg, 2);// You could also set other flags such as retained
```



## Subscribing to topics ( QoS0, QoS1)   

 > To receive data from a topic to which your device is ***Subscribed*** to :  

 ```c++ 
client.subscribe("<username/topic/Topic>"); // QoS 0 subscription (default)
 ```

```c++
client.subscribe("<username/topic/Topic>",1); // QoS 1 subscription 
```

