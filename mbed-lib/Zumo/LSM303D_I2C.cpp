#include "mbed.h"
#include "LSM303D_I2C.h"

LSM303D_I2C::LSM303D_I2C(PinName sda, PinName scl) :
		i2c(sda, scl) {
	i2c.frequency(100000);

	// CTRL0 : acc data rate 100Hz, Block data update disable, acc x,y,z enable
	write_reg(CTRL1, 0b01100111);
	// CTRL5 : temp en, high reso, data rate 100Hz, int latch disable
	write_reg(CTRL5, 0b11110100);
	// CTRL7 : mag Continuous-conversion mode, others default.
	write_reg(CTRL7, 0b00000000);
}

bool LSM303D_I2C::read_acc(short *x, short *y, short *z) {
	char bufL, bufH;
	bool result = true;
	result &= read_reg(OUT_X_L_A, &bufL);
	result &= read_reg(OUT_X_H_A, &bufH);
	*x = (short) ((bufH << 8) + bufL);
	result &= read_reg(OUT_X_L_A, &bufL);
	result &= read_reg(OUT_Y_H_A, &bufH);
	*y = (short) ((bufH << 8) + bufL);
	result &= read_reg(OUT_Z_L_A, &bufL);
	result &= read_reg(OUT_Z_H_A, &bufH);
	*z = (short) ((bufH << 8) + bufL);
	return result;
}

bool LSM303D_I2C::read_mag(short *x, short *y, short *z) {
	char bufL, bufH;
	bool result = true;
	result &= read_reg(OUT_X_L_M, &bufL);
	result &= read_reg(OUT_X_H_M, &bufH);
	*x = (short) ((bufH << 8) + bufL);
	result &= read_reg(OUT_X_L_M, &bufL);
	result &= read_reg(OUT_Y_H_M, &bufH);
	*y = (short) ((bufH << 8) + bufL);
	result &= read_reg(OUT_Z_L_M, &bufL);
	result &= read_reg(OUT_Z_H_M, &bufH);
	*z = (short) ((bufH << 8) + bufL);
	return result;
}


bool LSM303D_I2C::write_reg(char reg, char data) {
	char buf[2];
	buf[0] = reg;
	buf[1] = data;
	return i2c.write(0x3A, buf, 2);
}

bool LSM303D_I2C::read_reg(char reg, char *data) {
	char buf[1];
	buf[0] = reg;
	bool result = true;
	result &= i2c.write(0x3A, buf, 1, true);
	result &= i2c.read(0x3B, data, 1);
	return result;
}

