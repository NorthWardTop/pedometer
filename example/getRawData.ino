/**
 * https://www.basemu.com/mpu6050-interfacing-with-nodemcu.html
 * 
 */

#include <ESP8266WiFi.h>
#include <Wire.h>
#include <Arduino.h>

#include "ESPAsyncUDP.h"
#include "MyMpu6050.h"

const char * ssid = "OpenWrt";
const char * password = "1234567890";

AsyncUDP udp;


void setup()
{
	Serial.begin(9600);
	// Select SDA and SCL pins for I2C communication 
	// const uint8_t SCL = D6;
	// const uint8_t SDA = D7;
	Wire.begin();

	AccGyr.mpu6050Init();
	
	WiFi.mode(WIFI_STA);
	WiFi.begin(ssid, password);
	


	if (WiFi.waitForConnectResult() != WL_CONNECTED) {
		Serial.println("WiFi Failed");
		while(1) {
			delay(1000);
		}
	} else 
	    Serial.println("WiFi connected");
	if(udp.connect(IPAddress(192,168,1,169), 1234)) {
		Serial.println("UDP connected");
		udp.onPacket([](AsyncUDPPacket packet) {
			Serial.print("UDP Packet Type: ");
			Serial.print(packet.isBroadcast()?"Broadcast":packet.isMulticast()?"Multicast":"Unicast");
			Serial.print(", From: ");
			Serial.print(packet.remoteIP());
			Serial.print(":");
			Serial.print(packet.remotePort());
			Serial.print(", To: ");
			Serial.print(packet.localIP());
			Serial.print(":");
			Serial.print(packet.localPort());
			Serial.print(", Length: ");
			Serial.print(packet.length());
			Serial.print(", Data: ");
			Serial.write(packet.data(), packet.length());
			Serial.println();
			//reply to the client
			packet.printf("Got %u bytes of data", packet.length());
		});
		//Send unicast
		udp.print("Hello Server!");
	}
}

void loop()
{
	struct Mpu6050Data data;

	delay(1000);

	AccGyr.updateValue(&data);

	//Ax: -0.00 Ay: -0.00 Az: -0.00 T: 36.53 Gx: -0.01 Gy: -0.01 Gz: -0.01

	//Ax: -0.00 Ay: -0.00 Az: -0.00 T: 36.53 Gx: -0.01 Gy: -0.01 Gz: -0.01
	AccGyr.printRawData();


	// Send broadcast on port 1234
	udp.broadcastTo("Anyone here?", 1234);
}





// // I2C device class (I2Cdev) demonstration Arduino sketch for MPU6050 class
// // 10/7/2011 by Jeff Rowberg <jeff@rowberg.net>
// // Updates should (hopefully) always be available at https://github.com/jrowberg/i2cdevlib
// //
// // Changelog:
// //     2011-10-07 - initial release

// /* ============================================
// I2Cdev device library code is placed under the MIT license
// Copyright (c) 2011 Jeff Rowberg

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// ===============================================
// */

// // Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// // is used in I2Cdev.h
// #include <Wire.h>
// #include <ESP8266WiFi.h>
// // I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// // for both classes must be in the include path of your project
// #include "I2Cdev.h"
// #include "MPU6050.h"
// #include "ESPAsyncUDP.h"

// // class default I2C address is 0x68
// // specific I2C addresses may be passed as a parameter here
// // AD0 low = 0x68 (default for InvenSense evaluation board)
// // AD0 high = 0x69
// MPU6050 accelgyro;

// int16_t ax, ay, az;
// int16_t gx, gy, gz;
// int16_t t;

// // #define LED_PIN 13
// #define LED_PIN 0
// // #define LED_PIN 4

// bool blinkState = false;

// void setup() {
//     // join I2C bus (I2Cdev library doesn't do this automatically)
//     Wire.begin();

//     // initialize serial communication
//     // (38400 chosen because it works as well at 8MHz as it does at 16MHz, but
//     // it's really up to you depending on your project)
//     Serial.begin(38400);
// 	delay(1000);//
//     // initialize device
//     Serial.println("Initializing I2C devices...");
//     accelgyro.initialize();
    
//     Serial.println("Testing device connections...");
//     Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

//     // configure Arduino LED for
//     pinMode(LED_PIN, OUTPUT);
// }

// void loop() {
//     // read raw accel/gyro measurements from device
//     accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
//     t = accelgyro.getTemperature();
//     t = t/340+36.53;
//     // these methods (and a few others) are also available
//     //accelgyro.getAcceleration(&ax, &ay, &az);
//     //accelgyro.getRotation(&gx, &gy, &gz);

//     // display tab-separated accel/gyro x/y/z values
//     Serial.print("a/g/t:\t");
//     Serial.print(ax); Serial.print("\t");
//     Serial.print(ay); Serial.print("\t");
//     Serial.print(az); Serial.print("\t");
//     Serial.print(gx); Serial.print("\t");
//     Serial.print(gy); Serial.print("\t");
//     Serial.print(t); Serial.print("\t");
//     Serial.println(gz);

//     // blink LED to indicate activity
//     blinkState = !blinkState;
//     digitalWrite(LED_PIN, blinkState);
// 	delay(200);
// }







// /*MPU-60X0对陀螺仪和加速计分别用了三个16位的ADC，将其测量的模拟量转化为可输出的数字量。为了精确跟踪快速和慢速运动，
// 传感器的测量范围是可控的，陀螺仪可测范围为±250，±500，±1000，±2000°/秒(dps)，加速计可测范围为±2，±4，±8，±16g(重力加速度)。*/
// #include <Wire.h>

// // MPU6050 Slave Device Address
// const uint8_t MPU6050SlaveAddress = 0x68;

// // sensitivity scale factor respective to full scale setting provided in datasheet
// const uint16_t AccelScaleFactor = 16384;
// const uint16_t GyroScaleFactor = 131;

// // MPU6050 few configuration register addresses 配置寄存器地址
// const uint8_t MPU6050_REGISTER_SMPLRT_DIV   =  0x19;
// const uint8_t MPU6050_REGISTER_USER_CTRL    =  0x6A;
// const uint8_t MPU6050_REGISTER_PWR_MGMT_1   =  0x6B;
// const uint8_t MPU6050_REGISTER_PWR_MGMT_2   =  0x6C;
// const uint8_t MPU6050_REGISTER_CONFIG       =  0x1A;
// const uint8_t MPU6050_REGISTER_GYRO_CONFIG  =  0x1B;
// const uint8_t MPU6050_REGISTER_ACCEL_CONFIG =  0x1C;
// const uint8_t MPU6050_REGISTER_FIFO_EN      =  0x23;
// const uint8_t MPU6050_REGISTER_INT_ENABLE   =  0x38;
// const uint8_t MPU6050_REGISTER_ACCEL_XOUT_H =  0x3B;
// const uint8_t MPU6050_REGISTER_SIGNAL_PATH_RESET  = 0x68;

// int16_t AccelX, AccelY, AccelZ, Temperature, GyroX, GyroY, GyroZ;

// void setup() {
//   Serial.begin(9600);
//   Wire.begin();
//   //Wire.begin(D6, D7); //启动Wire库，并以主机或从机身份加入I2C总线
//   MPU6050_Init(); //配置MPU6050
// }

// void loop() {
//   double Ax, Ay, Az, T, Gx, Gy, Gz;
  
//   Read_RawValue(MPU6050SlaveAddress, MPU6050_REGISTER_ACCEL_XOUT_H);//读取寄存器函数(0x68,0x3B);
  
//   //divide each with their sensitivity scale factor 
//   Ax = (double)AccelX/AccelScaleFactor;
//   Ay = (double)AccelY/AccelScaleFactor;
//   Az = (double)AccelZ/AccelScaleFactor;
//   T = (double)Temperature/340+36.53; //temperature formula
//   Gx = (double)GyroX/GyroScaleFactor;
//   Gy = (double)GyroY/GyroScaleFactor;
//   Gz = (double)GyroZ/GyroScaleFactor;

//   Serial.print("Ax: "); Serial.print(Ax);
//   Serial.print(" Ay: "); Serial.print(Ay);
//   Serial.print(" Az: "); Serial.print(Az);
//   Serial.print(" T: "); Serial.print(T);
//   Serial.print(" Gx: "); Serial.print(Gx);
//   Serial.print(" Gy: "); Serial.print(Gy);
//   Serial.print(" Gz: "); Serial.println(Gz);

//   delay(100);
// }

// void I2C_Write(uint8_t deviceAddress, uint8_t regAddress, uint8_t data){
//   Wire.beginTransmission(deviceAddress);//例0x68
//   Wire.write(regAddress);               //例0x19
//   Wire.write(data);                     //例0x07
//   Wire.endTransmission();
// }

// // read all 14 register 读取全部14寄存器
// void Read_RawValue(uint8_t deviceAddress, uint8_t regAddress){
//   Wire.beginTransmission(deviceAddress);        //1.此函数使用给定地址开始向I2C从设备的传输;
//   Wire.write(regAddress);                       //2.此函数对要传输的字节进行排队;
//   Wire.endTransmission();                       //3.此函数进行传输;
//   Wire.requestFrom(deviceAddress, (uint8_t)14); //主机用于从从设备请求字节。然后可以使用available（）和read（）函数检索字节
//   AccelX = (((int16_t)Wire.read()<<8) | Wire.read());
//   AccelY = (((int16_t)Wire.read()<<8) | Wire.read());
//   AccelZ = (((int16_t)Wire.read()<<8) | Wire.read());
//   Temperature = (((int16_t)Wire.read()<<8) | Wire.read());
//   GyroX = (((int16_t)Wire.read()<<8) | Wire.read());
//   GyroY = (((int16_t)Wire.read()<<8) | Wire.read());
//   GyroZ = (((int16_t)Wire.read()<<8) | Wire.read());
// }

// //configure MPU6050 配置MPU6050
// void MPU6050_Init(){
//   delay(150);
//   I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_SMPLRT_DIV, 0x07);
//   I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_PWR_MGMT_1, 0x01);
//   I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_PWR_MGMT_2, 0x00);
//   I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_CONFIG, 0x00);
//   I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_GYRO_CONFIG, 0x00);//set +/-250 degree/second full scale
//   I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_ACCEL_CONFIG, 0x00);// set +/- 2g full scale
//   I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_FIFO_EN, 0x00);
//   I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_INT_ENABLE, 0x01);
//   I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_SIGNAL_PATH_RESET, 0x00);
//   I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_USER_CTRL, 0x00);
// }




// #include<Wire.h>
// const int MPU6050_addr=0x68;
// int16_t AccX,AccY,AccZ,Temp,GyroX,GyroY,GyroZ;
// void setup(){
//   Wire.begin();
//   Wire.beginTransmission(MPU6050_addr);
//   Wire.write(0x6B);
//   Wire.write(0);
//   Wire.endTransmission(true);
//   Serial.begin(9600);
// } 
// void loop(){
//   Wire.beginTransmission(MPU6050_addr);
//   Wire.write(0x3B);
//   Wire.endTransmission(false);
//   Wire.requestFrom(MPU6050_addr,14,true);
//   AccX=Wire.read()<<8|Wire.read();
//   AccY=Wire.read()<<8|Wire.read();
//   AccZ=Wire.read()<<8|Wire.read();
//   Temp=Wire.read()<<8|Wire.read();
//   GyroX=Wire.read()<<8|Wire.read();
//   GyroY=Wire.read()<<8|Wire.read();
//   GyroZ=Wire.read()<<8|Wire.read();
//   Serial.print("AccX = "); Serial.print(AccX);
//   Serial.print(" || AccY = "); Serial.print(AccY);
//   Serial.print(" || AccZ = "); Serial.print(AccZ);
//   Serial.print(" || Temp = "); Serial.print(Temp/340.00+36.53);
//   Serial.print(" || GyroX = "); Serial.print(GyroX);
//   Serial.print(" || GyroY = "); Serial.print(GyroY);
//   Serial.print(" || GyroZ = "); Serial.println(GyroZ);
//   delay(100);
// }