
// I want to create an ardunio ble application that sends 3 float values.  
// Please suggest improvements to the arduino code.  On the receiving side 
// a python script will write data to a file. Please help me with the python

#include <bluefruit.h>
#include "LSM6DS3.h"
#include "Wire.h"

//Create a instance of class LSM6DS3
LSM6DS3 myIMU(I2C_MODE, 0x6A);    //I2C device address 0x6A

// define the LED pins
const int LED_PINS[] = {LED_RED, LED_BLUE, LED_GREEN};
const int NUM_LEDS = sizeof(LED_PINS) / sizeof(LED_PINS[0]);
const char* IMU_SERVICE_UUID = "4401d1bb-95c6-461e-9579-cf6989cccddc";
const char* IMU_AX_UUID = "7b53dc33-a8ca-41af-a269-2eb2bb432b40";
const char* IMU_AY_UUID = "7b53dc33-a8ca-41af-a269-2eb2bb432b50";
const char* IMU_AZ_UUID = "7b53dc33-a8ca-41af-a269-2eb2bb432b60";
const char* IMU_GX_UUID = "7b53dc33-a8ca-41af-a269-2eb2bb432b70";
const char* IMU_GY_UUID = "7b53dc33-a8ca-41af-a269-2eb2bb432b80";
const char* IMU_GZ_UUID = "7b53dc33-a8ca-41af-a269-2eb2bb432b90";
const char* IMU_C_UUID = "7b53dc33-a8ca-41af-a269-2eb2bb432ba0";
const char* IMU_F_UUID = "7b53dc33-a8ca-41af-a269-2eb2bb432bb0";

BLEService ble_imu_service(IMU_SERVICE_UUID);
BLECharacteristic imu_ax(IMU_AX_UUID, BLERead, 4 , true);
BLECharacteristic imu_ay(IMU_AY_UUID, BLERead, 4 , true);
BLECharacteristic imu_az(IMU_AZ_UUID, BLERead, 4 , true);
BLECharacteristic imu_gx(IMU_GX_UUID, BLERead, 4 , true);
BLECharacteristic imu_gy(IMU_GY_UUID, BLERead, 4 , true);
BLECharacteristic imu_gz(IMU_GZ_UUID, BLERead, 4 , true);
BLECharacteristic imu_c(IMU_C_UUID, BLERead, 4 , true);
BLECharacteristic imu_f(IMU_F_UUID, BLERead, 4 , true);

void initLEDs();
void initSerial();
void startAdv(void);
void initBLE();
void initIMU();

void setup() 
{
  initSerial();
  initLEDs();
  initBLE();
  initIMU();
  digitalWrite(LED_GREEN, LOW);
}

void readIMU(float (&imu_data)[8]);
void writeBLEData(float (&imu_data)[8]);

void loop() 
{
  float imu_data[8];
  readIMU(imu_data);
  writeBLEData(imu_data);
  delay(1000);
}

void writeBLEData(float (&imu_data)[8])
{
  imu_ax.writeFloat(imu_data[0]);
  imu_ay.writeFloat(imu_data[1]);
  imu_az.writeFloat(imu_data[2]);
  imu_gx.writeFloat(imu_data[3]);
  imu_gy.writeFloat(imu_data[4]);
  imu_gz.writeFloat(imu_data[5]);
  imu_c.writeFloat(imu_data[6]);
  imu_f.writeFloat(imu_data[7]);
}

void readIMU(float (&imu_data)[8])
{
    imu_data[0] = myIMU.readFloatAccelX();
    imu_data[1] = myIMU.readFloatAccelY();
    imu_data[2] = myIMU.readFloatAccelZ();
    imu_data[3] = myIMU.readFloatGyroX();
    imu_data[4] = myIMU.readFloatGyroY();
    imu_data[5] = myIMU.readFloatGyroZ();
    imu_data[6] = myIMU.readTempC();
    imu_data[7] = myIMU.readTempF();

      //Accelerometer
    Serial.print("\nAccelerometer:\n");
    Serial.print(" X1 = ");
    Serial.println(imu_data[0], 4);
    Serial.print(" Y1 = ");
    Serial.println(imu_data[1], 4);
    Serial.print(" Z1 = ");
    Serial.println(imu_data[2], 4);

    //Gyroscope
    Serial.print("\nGyroscope:\n");
    Serial.print(" X1 = ");
    Serial.println(imu_data[3], 4);
    Serial.print(" Y1 = ");
    Serial.println(imu_data[4], 4);
    Serial.print(" Z1 = ");
    Serial.println(imu_data[5], 4);

    //Thermometer
    Serial.print("\nThermometer:\n");
    Serial.print(" Degrees C1 = ");
    Serial.println(imu_data[6], 4);
    Serial.print(" Degrees F1 = ");
    Serial.println(imu_data[7], 4);
}

void initIMU()
{
  if (myIMU.begin() != 0) {
    Serial.println("Device error");
  } else {
    Serial.println("Device OK!");
  }
}

void initLEDs() {
  // Initialize the LED pins as outputs.
  for (int i = 0; i < NUM_LEDS; i++) {
    pinMode(LED_PINS[i], OUTPUT);
    digitalWrite(LED_PINS[i], HIGH);    // turn the LED off by making the voltage HIGH
  }
}

void initSerial()
{
  Serial.begin(115200);
  Serial.println("Starting...");
}

void initBLE()
{
  Bluefruit.begin();
  Bluefruit.setName("Reflex-o-meter");

  // Configure and Start BLE IMU Service
  ble_imu_service.begin();
  imu_ax.begin();
  imu_ay.begin();
  imu_az.begin();
  imu_gx.begin();
  imu_gy.begin();
  imu_gz.begin();
  imu_c.begin();
  imu_f.begin();
  startAdv();
}

void startAdv(void)
{  
  // Advertising packet
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addName();
  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.addService(ble_imu_service);
  Bluefruit.Advertising.start(0);                // 0 = Don't stop advertising after n seconds
}

