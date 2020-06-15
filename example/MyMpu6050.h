#include <ESP8266WiFi.h>
#include <Wire.h>

#ifndef __MPU6050_H__
#define __MPU6050_H__


// MPU6050 few configuration register addresses
#define MPU6050_REGISTER_SMPLRT_DIV               0x19
#define MPU6050_REGISTER_USER_CTRL                0x6A
#define MPU6050_REGISTER_PWR_MGMT_1               0x6B
#define MPU6050_REGISTER_PWR_MGMT_2               0x6C
#define MPU6050_REGISTER_CONFIG                   0x1A
#define MPU6050_REGISTER_GYRO_CONFIG              0x1B
#define MPU6050_REGISTER_ACCEL_CONFIG             0x1C
#define MPU6050_REGISTER_FIFO_EN                  0x23
#define MPU6050_REGISTER_INT_ENABLE               0x38
#define MPU6050_REGISTER_ACCEL_XOUT_H             0x3B
#define MPU6050_REGISTER_SIGNAL_PATH_RESET        0x68




// MPU6050 Slave Device Address
const uint8_t MPU6050SlaveAddress = 0x68;

  

struct Mpu6050Data
{
    double Ax, Ay, Az, T, Gx, Gy, Gz;
};



class Mpu6050
{
private:
    /* data */
    int16_t AccelX, AccelY, AccelZ, Temperature, GyroX, GyroY, GyroZ;

    // sensitivity scale factor respective to full scale setting provided in datasheet 
    uint16_t AccelScaleFactor;
    uint16_t GyroScaleFactor;

public:
    Mpu6050(/* args */);
    ~Mpu6050();

    void mpu6050Init();
    void I2C_Write(uint8_t deviceAddress, uint8_t regAddress, uint8_t data);
    void Read_RawValue();
    void updateValue(struct Mpu6050Data *data);
    void printRawData();
    void printData(struct Mpu6050Data *data);
};

Mpu6050::Mpu6050(/* args */)
{
    mpu6050Init();
}

Mpu6050::~Mpu6050()
{
}


//configure MPU6050
void Mpu6050::mpu6050Init()
{
    delay(150);
    Serial.println("serial print: mpu6050 init");
    I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_SMPLRT_DIV, 0x07);
    I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_PWR_MGMT_1, 0x01);
    I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_PWR_MGMT_2, 0x00);
    I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_CONFIG, 0x00);
    I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_GYRO_CONFIG, 0x00);//set +/-250 degree/second full scale
    I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_ACCEL_CONFIG, 0x00);// set +/- 2g full scale
    I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_FIFO_EN, 0x00);
    I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_INT_ENABLE, 0x01);
    I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_SIGNAL_PATH_RESET, 0x00);
    I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_USER_CTRL, 0x00);

    AccelScaleFactor = 16384;
    GyroScaleFactor = 131;
}

//修改寄存器
void Mpu6050::I2C_Write(uint8_t deviceAddress, uint8_t regAddress, uint8_t data)
{
    Wire.beginTransmission(deviceAddress);
    Wire.write(regAddress);
    Wire.write(data);
    Wire.endTransmission();
}

//读mpu设备原始数据
// read all 14 register
void Mpu6050::Read_RawValue()
{
    Wire.beginTransmission(MPU6050SlaveAddress);
    Wire.write(MPU6050_REGISTER_ACCEL_XOUT_H);
    Wire.endTransmission();
    Wire.requestFrom(MPU6050SlaveAddress, (uint8_t)14);
    AccelX = (((int16_t)Wire.read()<<8) | Wire.read());
    AccelY = (((int16_t)Wire.read()<<8) | Wire.read());
    AccelZ = (((int16_t)Wire.read()<<8) | Wire.read());
    Temperature = (((int16_t)Wire.read()<<8) | Wire.read());
    GyroX = (((int16_t)Wire.read()<<8) | Wire.read());
    GyroY = (((int16_t)Wire.read()<<8) | Wire.read());
    GyroZ = (((int16_t)Wire.read()<<8) | Wire.read());
}


void Mpu6050::updateValue(struct Mpu6050Data *data)
{
    
    Read_RawValue();
    //divide each with their sensitivity scale factor
    data->Ax = (double)AccelX/AccelScaleFactor;
    data->Ay = (double)AccelY/AccelScaleFactor;
    data->Az = (double)AccelZ/AccelScaleFactor;
    data->T = (double)Temperature/340+36.53; //temperature formula
    data->Gx = (double)GyroX/GyroScaleFactor;
    data->Gy = (double)GyroY/GyroScaleFactor;
    data->Gz = (double)GyroZ/GyroScaleFactor;
}


void Mpu6050::printData(struct Mpu6050Data *data)
{
    Serial.print("Ax: "); Serial.print(data->Ax);
    Serial.print(" Ay: "); Serial.print(data->Ay);
    Serial.print(" Az: "); Serial.print(data->Az);
    Serial.print(" T: "); Serial.print(data->T);
    Serial.print(" Gx: "); Serial.print(data->Gx);
    Serial.print(" Gy: "); Serial.print(data->Gy);
    Serial.print(" Gz: "); Serial.println(data->Gz);
}


void Mpu6050::printRawData()
{

    Serial.print("Ax: "); Serial.print(AccelX);
    Serial.print(" Ay: "); Serial.print(AccelY);
    Serial.print(" Az: "); Serial.print(AccelZ);
    Serial.print(" T: "); Serial.print(Temperature);
    Serial.print(" Gx: "); Serial.print(GyroX);
    Serial.print(" Gy: "); Serial.print(GyroY);
    Serial.print(" Gz: "); Serial.println(GyroZ);
}


//导出对象即可使用
Mpu6050 AccGyr;
extern Mpu6050 AccGyr;

#endif // __MPU6050_H__
