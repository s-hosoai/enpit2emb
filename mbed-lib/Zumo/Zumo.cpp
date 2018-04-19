#include "Zumo.h"

const int MAX_SENSOR_VALUE = 1024;
const int IR_THRESHOLD = 100;

static int constrain(int input, int min, int max);

static DigitalInOut ir_pins[] = {
	DigitalInOut(D5),
	DigitalInOut(P11_14),
		DigitalInOut(P11_12),
	DigitalInOut(D11),
	DigitalInOut(P11_15),
	DigitalInOut(D4)
};
static DigitalOut emitter(A4);
static Timer timer;

DigitalOut dir_left(D8);
DigitalOut dir_right(D7);
PwmOut pwm_left(P5_0);
PwmOut pwm_right(P8_14);
I2C i2c(P1_3, P1_2);

void Zumo::driveTank(int left, int right) {
	int _lmotor = constrain(left, -255, 255);
	int _rmotor = constrain(right, -255, 255);

	if (_lmotor < 0){
		dir_left = 1;
		_lmotor = -_lmotor;
	}else{
		dir_left = 0;
	}
	if(_rmotor < 0){
		dir_right = 1;
		_rmotor = -_rmotor;
	}else{
		dir_right = 0;
	}
	pwm_left.write(_lmotor/255.0f);
	pwm_right.write(_rmotor/255.0f);
}

void Zumo::readAnalogIrValue(unsigned int *values) {
	int i;
	emitter = 1;
	wait_us(200);
	for (i = 0; i < 6; i++) {
		values[i] = MAX_SENSOR_VALUE - 1;
		ir_pins[i].output();
		ir_pins[i] = 1;
	}
	for (i = 0; i < 6; i++) {
		ir_pins[i].input();
		ir_pins[i] = 0;
	}
	timer.start();
	timer.reset();
	unsigned int time = 0;
	while (timer.read_us() < MAX_SENSOR_VALUE) {
		time = timer.read_us();
		for (i = 0; i < 6; i++) {
			if (ir_pins[i] == 0 && time < values[i]) {
				values[i] = time;
			}
		}
	}
	timer.stop();
	emitter = 0;
	wait_us(200);
}

void Zumo::readIr(IrBitField_T &irbits) {
	unsigned int irvalues[6];
	readAnalogIrValue(irvalues);
	irbits.right = (irvalues[0] > IR_THRESHOLD);
	irbits.center = (irvalues[3] > IR_THRESHOLD);
	irbits.left = (irvalues[5] > IR_THRESHOLD);
}

static int constrain(int input, int min, int max){
	if(input < min){
		return min;
	}else if (max < input){
		return max;
	}else{
		return input;
	}
}

//void getAcceleration() {
//	char wBuf[1];
//	char rBuf[6];
//	wBuf[0] = 0x0F;
//	i2c.write(0x3A, wBuf, 1, true);
//	dly_tsk(70);
//	i2c.read(0x3B, rBuf, 1, false);
//	pc.printf("%d\r\n", rBuf[0]);
////	i2c.write(0x3A, wBuf, 1, true);
////	dly_tsk(70);
////	i2c.read(0x3B, rBuf, 6, false);
////	dly_tsk(70);
////	pc.printf("x=%d, y=%d, z=%d\n\r", (rBuf[1] << 8) + rBuf[0],
////			(rBuf[3] << 8) + rBuf[2], (rBuf[5] << 8) + rBuf[4]);
//}

