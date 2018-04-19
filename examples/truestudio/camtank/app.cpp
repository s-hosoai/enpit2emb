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

}
