#include "mbed.h"

DigitalInOut ir_pins[] = {
    DigitalInOut(D5),
    DigitalInOut(A2),
    DigitalInOut(A0),
    DigitalInOut(D11),
    DigitalInOut(A3),
    DigitalInOut(D4)
};
DigitalOut emitter(A4);
Timer timer;

DigitalOut dir_left(D8);
DigitalOut dir_right(D7);
PwmOut pwm_left(P5_0);
PwmOut pwm_right(P8_14);
PwmOut pwm_pan(P5_3);
PwmOut pwm_tilt(P3_8);
DigitalOut led(LED1);
Serial pc(USBTX, USBRX);


void irRead(unsigned int *sensor_values){
    unsigned int i;
    emitter = 1;
    wait_us(200);
    for(i = 0; i < 6; i++){
        sensor_values[i] = 1023;
        ir_pins[i].output();
        ir_pins[i] = 1;
    }
    wait_us(10);
    for(i = 0; i < 6; i++){
        ir_pins[i].input();
        ir_pins[i] = 0;
    }
    timer.start();
    timer.reset();
    unsigned int time = 0;
    while(timer.read_us() < 1024){
        time = timer.read_us();
        for(i = 0; i < 6; i++){
            if(ir_pins[i] == 0 && time < sensor_values[i]){
                sensor_values[i] = time;
            }
        }
    }
    timer.stop();
//    readPrivate(sensor_values);
    emitter = 0;
    wait_us(200);
}

void tankDrive(int leftMotor, int rightMotor){
    int _leftMotor = leftMotor;
    int _rightMotor = rightMotor;
    if(_leftMotor < -255){
        _leftMotor = -255;
    }else if(_leftMotor > 255){
        _leftMotor = 255;
    }
    if(_rightMotor < -255){
        _rightMotor = -255;
    }else if(_rightMotor > 255){
        _rightMotor = 255;
    }
    if(_leftMotor < 0){
        dir_left = -1;
        _leftMotor = -_leftMotor;
    }else{
        dir_left = 0;
    }
    
    if(rightMotor < 0){
        dir_right = -1;
        _rightMotor = -_rightMotor;
    }else{
        dir_right = 0;
    }
    pwm_left.write(_leftMotor/255.0f);
    pwm_right.write(_rightMotor/255.0f);
    pc.printf("%f:%f",_leftMotor/255.0f, _rightMotor/255.0f);
}

int main() {
    pc.baud(9600);
    unsigned int irdata[6];

    while(1) {
        irRead(irdata);
        pc.printf("%d, %d, %d, %d, %d, %d\r\n", irdata[0], irdata[1], irdata[2], irdata[3], irdata[4], irdata[5]);
        wait(1.0);
        
//        led = 1;
//        tankDrive(200,200);
//        wait(2.0);
//        tankDrive(-200,200);
//        wait(2.0);
//        led = 0;
//        tankDrive(-200,-200);
//        wait(2.0);
//        tankDrive(200,-200);
//        wait(2.0);
    }
}
