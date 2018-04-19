#include "Milkcocoa.h"

#if 0
#if 0
#include "SoftSerialSendOnry.h"
extern SoftSerialSendOnry pc;
#else
extern Serial pc;
#endif
#define DBG(x)  x
#else
#define DBG(x)
#endif

DataElement::DataElement() {
	json_msg[0] = '\0';
	strcpy(json_msg,"{\"params\":{");
}

DataElement::DataElement(char *json_string) {
	json_msg[0] = '\0';
	strcpy(json_msg,json_string);
}

void DataElement::setValue(const char *key, const char *v) {
	char json_string[64];
	if( json_msg[strlen(json_msg)-1] != '{' )
	{
		strcat(json_msg,",");
	}
	sprintf(json_string,"\"%s\":\"%s\"",key,v);
	strcat(json_msg,json_string);
}

void DataElement::setValue(const char *key, int v) {
	char json_string[64];
	if( json_msg[strlen(json_msg)-1] != '{' )
	{
		strcat(json_msg,",");
	}
	sprintf(json_string,"\"%s\":\"%d\"",key,v);
	strcat(json_msg,json_string);
}

void DataElement::setValue(const char *key, double v) {
	char json_string[64];
	if( json_msg[strlen(json_msg)-1] != '{' )
	{
		strcat(json_msg,",");
	}
	sprintf(json_string,"\"%s\":\"%f\"",key,v);
	strcat(json_msg,json_string);
}

char *DataElement::getString(const char *key) {
    static char _word[64];
    char *p;
	int i=0;
	
	strcpy(_word , "\"\0");
    strcat(_word , key );
    strcat(_word , "\"" );
 
    p = strstr( (char*)json_msg , _word ) + 2 + strlen(_word);
	
    while( (p[i] != ',')&&(p[i] != '\n')&&(p[i] != '\"') )
    {
        _word[i] = p[i];
        i++;
    }
    _word[i] = '\0';
	
	return _word;
}

int DataElement::getInt(const char *key) {
	return atoi(getString(key));
}

float DataElement::getFloat(const char *key) {
	return atof(getString(key));
}

char *DataElement::toCharArray() {
	if( json_msg[strlen(json_msg)-1] != '{' )
	{
		strcat(json_msg,"}");
	}
	strcat(json_msg,"}");
	
	return(json_msg);
}

Milkcocoa::Milkcocoa(MClient *_client, const char *host, uint16_t port, const char *_app_id, const char *client_id) {
	
	client = _client;
	strcpy(servername,host);
	portnum = port;
	app_id = _app_id;
	strcpy(_clientid,client_id);
	strcpy(username,"sdammy");
	strcpy(password,app_id);

}

Milkcocoa::Milkcocoa(MClient *_client, const char *host, uint16_t port, const char *_app_id, const char *client_id, char *_session) {
	
	client = _client;
	strcpy(servername,host);
	portnum = port;
	app_id = _app_id;
	strcpy(_clientid,client_id);
	strcpy(username,_session);
	strcpy(password,app_id);

}

Milkcocoa* Milkcocoa::createWithApiKey(MClient *_client, const char *host, uint16_t port, const char *_app_id, const char *client_id, char *key, char *secret) {
	char session[60];
	sprintf(session, "k%s:%s", key, secret);
	return new Milkcocoa(_client, host, port, _app_id, client_id, session);
}

void Milkcocoa::connect() {

	if(client->isConnected())
		return;
		
	if(client->connect(servername, portnum)!=0) {
		DBG(pc.printf("Network connect err\r\n");)
		return;
	}

	MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
	data.keepAliveInterval = 20;
	data.cleansession = 1;
	data.MQTTVersion = 4;
	data.clientID.cstring = _clientid;
	data.username.cstring = username;
	data.password.cstring = password;

	if (client->connect(data) != 0)  {
		DBG(pc.printf("Milkcocoa connect err\r\n");)
		return;
	}
}

bool Milkcocoa::push(const char *path, DataElement dataelement) {
	char topic[100];
	char *buf;
	MQTT::Message message;

	sprintf(topic, "%s/%s/push", app_id, path);

	message.qos = MQTT::QOS0;
	message.retained = 0;
	message.dup = false;
	buf = dataelement.toCharArray();
	message.payload = (void*)buf;
	message.payloadlen = strlen(buf);
	if(client->publish(topic, message)!=0)
		return(false);
	
	return true;
}

bool Milkcocoa::send(const char *path, DataElement dataelement) {
	char topic[100];
	char *buf;
	MQTT::Message message;

	sprintf(topic, "%s/%s/send", app_id, path);
	message.qos = MQTT::QOS0;
	message.retained = 0;
	message.dup = false;
	buf = dataelement.toCharArray();
	message.payload = (void*)buf;
	message.payloadlen = strlen(buf);
	if(client->publish(topic, message)!=0)
		return false;
	
	return true;
}

void Milkcocoa::loop() {
	connect();
	client->yield(RECV_TIMEOUT);
}

bool Milkcocoa::on(const char *path, const char *event, GeneralFunction cb) {
	MilkcocoaSubscriber *sub = new MilkcocoaSubscriber(cb);
	sprintf(sub->topic, "%s/%s/%s", app_id, path, event);

	if (client->subscribe(sub->topic, MQTT::QOS0, cb) != 0)  {
		DBG(pc.printf("Milkcocoa subscribe err\r\n");)
		return false;
	}
	for (int i=0; i<MILKCOCOA_SUBSCRIBERS; i++) {
		if (milkcocoaSubscribers[i] == sub) {
			return false;
		}
	}
	for (int i=0; i<MILKCOCOA_SUBSCRIBERS; i++) {
		if (milkcocoaSubscribers[i] == 0) {
			milkcocoaSubscribers[i] = sub;
			return true;
		}
	}
	return true;
}

MilkcocoaSubscriber::MilkcocoaSubscriber(GeneralFunction _cb) {
  cb = _cb;
}