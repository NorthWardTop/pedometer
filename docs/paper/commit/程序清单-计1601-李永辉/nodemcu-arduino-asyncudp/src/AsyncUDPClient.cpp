// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#include <string>
#include <Wire.h>
#include <ESP8266WiFi.h>
// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"
#include "MPU6050.h"
#include "ESPAsyncUDP.h"


// 网络相关///////////////////////////////////////////////////////
#define SSID            "PDCN0"
#define PASSWORD        "1234567890"
#define SERVER_PORT     1234

AsyncUDP udp;

//传感器相关///////////////////////////////////////////////////////
// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for InvenSense evaluation board)
// AD0 high = 0x69
MPU6050 accelgyro;

struct raw_data {
    int16_t ax, ay, az;
    int16_t gx, gy, gz;
    int16_t t;
}data;



void setup() {
    // join I2C bus (I2Cdev library doesn't do this automatically)
    Wire.begin();

    // initialize serial communication
    // (38400 chosen because it works as well at 8MHz as it does at 16MHz, but
    // it's really up to you depending on your project)
    Serial.begin(38400);

    // initialize device
    Serial.println("\nInitializing I2C devices...");
    accelgyro.initialize();
    Serial.println("Testing device connections...");
    Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

    // WIFI 相关配置
	WiFi.mode(WIFI_STA);
	WiFi.begin(SSID, PASSWORD);
	if (WiFi.waitForConnectResult() != WL_CONNECTED) {
		Serial.println("WiFi Failed");
		while(1) {
			delay(1000);
		}
	} else {
        Serial.println("WiFi connected");
    }
}


void loop() {
    // read raw accel/gyro measurements from device
    accelgyro.getMotion6(&data.ax, &data.ay, &data.az, &data.gx, &data.gy, &data.gz);
    data.t = accelgyro.getTemperature();
    // these methods (and a few others) are also available
    //accelgyro.getAcceleration(&ax, &ay, &az);
    //accelgyro.getRotation(&gx, &gy, &gz);

    // display tab-separated accel/gyro x/y/z values
    Serial.print("a/g/t:\t");
    Serial.print(data.ax); Serial.print("\t");
    Serial.print(data.ay); Serial.print("\t");
    Serial.print(data.az); Serial.print("\t");
    Serial.print(data.gx); Serial.print("\t");
    Serial.print(data.gy); Serial.print("\t");
    Serial.print(data.gz);Serial.print("\t");
    Serial.println(data.t);

    //通过广播发送, 一个发送端多个接收端
    udp.broadcastTo((uint8_t *)&data, sizeof(data), SERVER_PORT);

    // //发送到树莓派
    // udp.writeTo((uint8_t*)&data, sizeof(data), IPAddress(192,168,123,185), SERVER_PORT);
    
    // //发送到python udp服务器
    // udp.writeTo((uint8_t*)&data, sizeof(data), IPAddress(192,168,123,14), SERVER_PORT);

    // //发送到PDCN0路由器 udp服务器
    // udp.writeTo((uint8_t*)&data, sizeof(data), IPAddress(192,168,123,1), SERVER_PORT);


    delay(40);
}




