#ifndef _MCLIENT_H_
#define _MCLIENT_H_

#define TARGET_GRPEACH_B3595

#if defined(TARGET_UBLOX_C027)
 #define USE_MDM
#elif defined(TARGET_LPC1768) || defined(TARGET_K64F) || defined(TARGET_W7500x)
 #define USE_ETHERNET
#elif defined(TARGET_GRPEACH_B3595)
 #define USE_BP3595
#else 
 #define USE_ESP8266
#endif

#include "mbed.h"
#if defined(USE_ESP8266)
 #include "MQTTESP8266.h"
#elif defined(USE_BP3595)
 #include "MQTTBP3595.h"
#elif defined(USE_MDM)
 #include "MQTTMDM.h"
#else 
 #include "MQTTEthernet.h"
#endif
#include "MQTTClient.h"

class MClient
{
 public:
    typedef void (*messageHandler)(MQTT::MessageData&);
#ifdef USE_ESP8266
	MClient(MQTTESP8266 *ipstack) : client(MQTT::Client<MQTTESP8266, Countdown>(*ipstack))
#elif defined(USE_BP3595)
	MClient(MQTTBP3595 *ipstack) : client(MQTT::Client<MQTTBP3595, Countdown>(*ipstack))
#elif defined(USE_MDM)
	MClient(MQTTMDM *ipstack) : client(MQTT::Client<MQTTMDM, Countdown>(*ipstack))
#else
	MClient(MQTTEthernet *ipstack) : client(MQTT::Client<MQTTEthernet, Countdown>(*ipstack))
#endif
	{
		_ipstack = ipstack;
	}
    void setDefaultMessageHandler(messageHandler mh)
    {
        client.setDefaultMessageHandler(mh);
    }
    int connect(char* host,int port)
	{
		return _ipstack->connect(host, port);
	}
    int connect()
	{
		return client.connect();
	}
    int connect(MQTTPacket_connectData& options)
	{
		return client.connect(options);
	}
    int publish(const char* topicName, MQTT::Message& message)
    {
		return client.publish(topicName,message);
	}
    int publish(const char* topicName, void* payload, size_t payloadlen, enum MQTT::QoS qos = MQTT::QOS0, bool retained = false)
	{
		return client.publish(topicName,payload,payloadlen,qos,retained);
	}
    int publish(const char* topicName, void* payload, size_t payloadlen, unsigned short& id, enum MQTT::QoS qos = MQTT::QOS1, bool retained = false)
	{
		return client.publish(topicName,payload,payloadlen,id,qos,retained);
	}
    int subscribe(const char* topicFilter, enum MQTT::QoS qos, messageHandler mh)
	{
		return client.subscribe(topicFilter,qos,mh);
	}
    int unsubscribe(const char* topicFilter)
	{
		return client.unsubscribe(topicFilter);
	}
    int disconnect()
	{
		return client.disconnect();
	}
    int yield(unsigned long timeout_ms = 1000L)
	{
		return client.yield(timeout_ms);
	}
    bool isConnected()
    {
        return client.isConnected();
    }
private:
#ifdef USE_ESP8266
	MQTT::Client<MQTTESP8266, Countdown> client;
  	MQTTESP8266 *_ipstack;
#elif defined(USE_BP3595)
	MQTT::Client<MQTTBP3595, Countdown> client;
  	MQTTBP3595 *_ipstack;
#elif defined(USE_MDM)
	MQTT::Client<MQTTMDM, Countdown> client;
  	MQTTMDM *_ipstack;
#else
	MQTT::Client<MQTTEthernet, Countdown> client;
  	MQTTEthernet *_ipstack;
#endif
};

#endif