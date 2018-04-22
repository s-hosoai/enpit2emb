#include <kernel.h>
#include "kernel_cfg.h"
#include "app.h"

#include "Zumo.h"
#include "mbed.h"
#include "app_config.h"
#include "GR_PEACH_WlanBP3595.h"

#define LWIP_DBG_ON
GR_PEACH_WlanBP3595 wlan;
Serial pc(USBTX, USBRX);
Zumo zumo;

void task_main(intptr_t exinf) {
	pc.baud(115200);
	pc.printf("\r\ninitializing\r\n");
	int ret = wlan.init(IP_ADDRESS, SUBNET_MASK, DEFAULT_GATEWAY);
	if(ret!=0){
		pc.printf("wifi initialize error\r\n");
		return;
	}

	pc.printf("wlan connecting\r\n");
	ret = wlan.connect(WLAN_SSID, WLAN_PSK);
	if(ret!=0){
		pc.printf("wifi connect error\r\n");
		return;
	}

	TCPSocketConnection socket;
	socket.set_blocking(true, 100);
	pc.printf("socket connecting\r\n");
	ret = socket.connect(SERVER_ADDRESS, SERVER_PORT);
	if(ret!=0){
		pc.printf("socket connect error\r\n");
		return;
	}
	
	short x, y, z;
    unsigned int irValues[6];
    int error;
	while(true){
		char accLog[50];
		char irLog[50];
        zumo.getAcceleration(&x, &y, &z);
        sprintf(accLog, "Acceleration : %d, %d, %d\r\n",x, y, z); 
        pc.printf("%s", accLog);
    	ret = socket.send_all(accLog, strlen(accLog));
    	if(ret<0){
    		pc.printf("socket send error : %d\r\n", ret);
    	}
    	
        zumo.readAnalogIrValue(irValues);
        error = irValues[3]-500;
		if (error > 50) {
			error = 50;
		} else if (error < -50) {
			error = -50;
		}
		
        sprintf(irLog, "IR[3] value : %d\r\n", irValues[3]);
        pc.printf("%s", irLog);
    	ret = socket.send_all(irLog, strlen(irLog));
    	if(ret<0){
    		pc.printf("socket send error : %d\r\n", ret);
    	}
		zumo.driveTank(100 + error, 100 - error);
		
	}
	
	socket.close();
	wlan.disconnect();
}
