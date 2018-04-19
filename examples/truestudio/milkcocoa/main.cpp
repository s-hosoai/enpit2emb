/*
Warning!
  When exporting and using it, increase the following stack size.
  
  [EthernetInterface/lwip/lwipopts.h]---------
  #define TCPIP_THREAD_STACKSIZE      1024
  ->
  #define TCPIP_THREAD_STACKSIZE      2048
  --------------------------------------------
*/

/*
This works with the following library.
  mbed-rtos : revision 115
*/

#include "mbed.h"
#include "Milkcocoa.h"

// Wi-Fi SSID
#define WLAN_SSID                       ("WLAN_SSID")
// Wi-Fiパスワード
#define WLAN_PSK                        ("WLAN_PSK")
// APP_ID
#define MILKCOCOA_APP_ID                "MILKCOCOA_APP_ID"

#define MILKCOCOA_DATASTORE             "zumo/sensor"
#define MILKCOCOA_MESSAGE_DATASTORE     "zumo/message"
#define MILKCOCOA_PUSH_DATASTORE        "zumo/remotecontrol"
#define MILKCOCOA_SERVERPORT            1883

extern void onpush(MQTT::MessageData& md);

const char MQTT_SERVER[]  = MILKCOCOA_APP_ID ".mlkcca.com";
const char MQTT_CLIENTID[] = __TIME__ MILKCOCOA_APP_ID;

DigitalOut  red_led(LED1);              // On: error
DigitalOut  green_led(LED2);            // On: WLAN has been connected

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

DigitalOut led_r(LED1);
DigitalOut led_g(LED2);
DigitalOut led_b(LED3);

// zumoの機器ID
const char zumoId[] = "zumo123456789";
// 初期起動
int initFlag = 0;
// スピード
int baseSpeed = 50;
int leftPower = 50;
int rightPower = 50;
// インターバル待ち秒数
int intervalSecLimit = 10;
// インターバルタイマー
Timer intervalTimer;
// 起動からの時間計測
Timer exeTimer;

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

void ledStatus(int r,int g,int b){
    led_r = r;
    led_g = g;
    led_b = b;
}

/** Main function
 *
 */
int main() {
    pc.baud(9600);
    
    // start
    intervalTimer.start();
    exeTimer.start();
    ledStatus(1,1,1);
    
    pc.printf("[[Milkcocoa mbed version]]\n\r");
    
    // connecting
    ledStatus(0,1,0);
    
    pc.printf("--- WLAN CONNECT START ---\n\r");
    
    MQTTBP3595 *ipstack = new MQTTBP3595(WLAN_SSID, WLAN_PSK);
    
    MClient *client = new MClient(ipstack);
    
    Milkcocoa *milkcocoa = new Milkcocoa(client, MQTT_SERVER, MILKCOCOA_SERVERPORT, MILKCOCOA_APP_ID, MQTT_CLIENTID);

    pc.printf("--- Milkcocoa start ---\n\r");
    
    milkcocoa->connect();
    pc.printf("milkcocoa base connected\n\r");
    
    // connected
    ledStatus(0,0,1);
    pc.printf(".");
    wait(1.0);
    ledStatus(0,0,0);
    pc.printf(".");
    wait(1.0);
    ledStatus(0,0,1);
    pc.printf(".");
    wait(1.0);
    ledStatus(0,0,0);
    pc.printf(".");
    wait(1.0);
    ledStatus(0,0,1);
    wait(1.0);  // 5秒程度は待たないとうまくいかない
    
    pc.printf("\n\r");
    pc.printf("milkcocoa onpush event connected\n\r");
    pc.printf("push -> topic:");
    pc.printf(MILKCOCOA_PUSH_DATASTORE);
    pc.printf("\n\r");
    int milkcocoa_ok = milkcocoa->on(MILKCOCOA_PUSH_DATASTORE, "push", onpush);
    pc.printf("%d\n\r",milkcocoa_ok);
    
    // connect status
    ledStatus(0,milkcocoa_ok,1);

    while(1) {
        // onpush検知のためにも回す必要がある
        milkcocoa->loop();
        // 初回起動だけメッセージを飛ばす
        if( initFlag == 0 ){
            initFlag = 1;
            DataElement msg_elem = DataElement();
            pc.printf("--- ZUMO_START ---\n\r");
            msg_elem.setValue("status", 1);
            msg_elem.setValue("onpush", milkcocoa_ok );
            msg_elem.setValue("et", (int)exeTimer.read() );
            milkcocoa->push(MILKCOCOA_MESSAGE_DATASTORE, msg_elem);
        }
        // intervalSecLimit 指定秒ごとに疎通データを送る
        if( intervalTimer.read() > intervalSecLimit ){
            pc.printf("--- ZUMO_SEND ---\n\r");
            DataElement elem = DataElement();
            elem.setValue("status", 2);
            elem.setValue("et", (int)exeTimer.read() );
            milkcocoa->push(MILKCOCOA_DATASTORE, elem);
            intervalTimer.reset();
        }
        
        // wait
        wait(0.5);
    }
}

void onpush(MQTT::MessageData& md)
{
    MQTT::Message &message = md.message;
    DataElement de = DataElement((char*)message.payload);
    pc.printf("\n\r");
    pc.printf("[onpush]\n\r");
    pc.printf((char*)message.payload);
    pc.printf("\n\r");
    const char *value = de.getString("v");
    pc.printf("%d\n\r",de.getInt("v"));
    pc.printf(value);
    pc.printf("\n\r");
    
    int v = de.getInt("v");
    
    if( v == 0 ){
        pc.printf("mode 0 \n\r");
        leftPower = 0;
        rightPower = 0;
        tankDrive(baseSpeed * leftPower,baseSpeed * rightPower);
    } else if( v == 1 ){
        pc.printf("mode 1 \n\r");
        leftPower = 1;
        rightPower = 1;
        tankDrive(baseSpeed * leftPower,baseSpeed * rightPower);
    } else if( v == 2 ){
        pc.printf("mode 2 \n\r");
        leftPower = -1;
        rightPower = -1;
        tankDrive(baseSpeed * leftPower,baseSpeed * rightPower);
    } else if( v == 3 ){
        pc.printf("mode 3 \n\r");
        leftPower = 1;
        rightPower = -1;
        tankDrive(baseSpeed * leftPower,baseSpeed * rightPower);
    } else if( v == 4 ){
        pc.printf("mode 4 \n\r");
        leftPower = -1;
        rightPower = 1;
        tankDrive(baseSpeed * leftPower,baseSpeed * rightPower);
    } else if( v == 5 ){
        pc.printf("mode 5 \n\r");
        baseSpeed += 10;
        tankDrive(baseSpeed * leftPower,baseSpeed * rightPower);
    } else if( v == 6 ){
        pc.printf("mode 6 \n\r");
        baseSpeed -= 10;
        tankDrive(baseSpeed * leftPower,baseSpeed * rightPower);
    } else if( v == 7 ){
        
    } else if( v == 8 ){
        
    } else if( v == 9 ){
        
    } else if( v == 90 ){
        
    } else if( v == 91 ){
        
    } else if( v == 92 ){
        
    }
    
}


