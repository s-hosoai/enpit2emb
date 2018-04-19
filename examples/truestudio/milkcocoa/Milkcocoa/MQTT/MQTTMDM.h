
#if !defined(MQTTMDM_H)
#define MQTTMDM_H

#include "MQTTmbed.h"
#include "MDM.h"
#include "MQTTSocket.h"

class MQTTMDM : public MQTTSocket
{
public:    
    MQTTMDM(const char* simpin = NULL,const char* apn = NULL, const char* username = NULL,const char* password = NULL)
    {
        mdm.connect(simpin, apn,username,password);
    }
    
    MDMSerial& getEth()
    {
        return mdm;
    }
    
    void reconnect()
    {
        mdm.disconnect();
    }
    
private:

    MDMSerial mdm;
    
};


#endif
