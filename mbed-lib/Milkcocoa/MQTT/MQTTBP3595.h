#if !defined(MQTTBP3595_H)
#define MQTTBP3595_H

#include "MQTTmbed.h"
#include "MQTTSocket.h"
#include "GR_PEACH_WlanBP3595.h"
#include "EthernetInterface.h"

class MQTTBP3595 : public MQTTSocket
{
public:    
    MQTTBP3595(char* wlanssid, char* wlanpsk)
    {
        wlan.init();                          // Use DHCP
        wlan.connect(wlanssid, wlanpsk);
        ssid=wlanssid;
        psk=wlanpsk;
    }
    
//    EthernetInterface& getEth()
//    {
//        return (EthernetInterface&)wlan;
//    }
    
    void reconnect()
    {
        wlan.connect(ssid, psk);  // nothing I've tried actually works to reconnect 
    }
    
private:
    GR_PEACH_WlanBP3595 wlan;
    char* ssid;
    char* psk;
};
#endif
