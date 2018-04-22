#ifndef ZUMO_H_
#define ZUMO_H_

#include "mbed.h"

typedef struct IrBitField_S {
	int left :1;
	int center :1;
	int right :1;
} IrBitField_T;

class Zumo {
public:
	void driveTank(int left, int right);
	void readIr(IrBitField_T &irbits);
	void readAnalogIrValue(unsigned int *values);
	void getAcceleration(short *x, short *y, short *z);
	void getMagetism(short *x, short *y, short *z);
	void setBuzzerHz(float hz);
	void buzzerOn();
	void buzzerOff();
private:
};



#endif // ZUMO_H_
