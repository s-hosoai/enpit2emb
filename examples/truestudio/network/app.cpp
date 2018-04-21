#include <kernel.h>
#include "kernel_cfg.h"
#include "app.h"

#include "mbed.h"

#include "app_config.h"

#include "GR_PEACH_WlanBP3595.h"

GR_PEACH_WlanBP3595 wlan;
Serial pc(USBTX, USBRX);

void task_main(intptr_t exinf) {
	pc.baud(115200);
	const char *buf;
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
	pc.printf("socket connecting\r\n");
	ret = socket.connect(SERVER_ADDRESS, SERVER_PORT);
	if(ret!=0){
		pc.printf("socket connect error\r\n");
		return;
	}

	char* data = strdup("a");
	ret = socket.send(data, strlen(data));
	if(ret!=0){
		pc.printf("socket send error\r\n");
		return;
	}

	socket.close();
	wlan.disconnect();
}
