#ifndef _MILKCOCOA_H_
#define _MILKCOCOA_H_

#include "mbed.h"
#include "MQTTmbed.h"
#include "MQTTClient.h"
#include "MClient.h"

#define RECV_TIMEOUT	500
#define MILKCOCOA_SUBSCRIBERS 8

class DataElement {
  public:
    DataElement();
    DataElement(char *json_string);
    void setValue(const char *key, const char *v);
    void setValue(const char *key, int v);
    void setValue(const char *key, double v);
    char *toCharArray();
    char *getString(const char *key);
    int getInt(const char *key);
    float getFloat(const char *key);

  private:
	char json_msg[256];
};

typedef void (*GeneralFunction) (MQTT::MessageData& elem);

class MilkcocoaSubscriber {
	public:
		GeneralFunction cb;
    char topic[100];
		MilkcocoaSubscriber(GeneralFunction _cb);
};

class Milkcocoa {
 public:
 
//  Milkcocoa(const char *host, uint16_t port, const char *_app_id, const char *client_id);
  Milkcocoa(MClient *_client, const char *host, uint16_t port, const char *_app_id, const char *client_id);
  Milkcocoa(MClient *_client, const char *host, uint16_t port, const char *_app_id, const char *client_id, char *_session);
  static Milkcocoa* createWithApiKey(MClient *_client, const char *host, uint16_t port, const char *_app_id, const char *client_id, char *key, char *secret);
  void connect();
  void loop();
  bool push(const char *path, DataElement dataelement);
  bool send(const char *path, DataElement dataelement);
  bool on(const char *path, const char *event, GeneralFunction cb);

private:
  char servername[64];
  int16_t portnum;
  char _clientid[64];
  char username[32];
  char password[32];
  const char *app_id;

  MClient *client;
  GeneralFunction _cb;
  MilkcocoaSubscriber *milkcocoaSubscribers[MILKCOCOA_SUBSCRIBERS];
};


#endif
