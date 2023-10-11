
// I want to create an ardunio ble application that sends 3 float values.  
// Please suggest improvements to the arduino code.  On the receiving side 
// a python script will write data to a file. Please help me with the python

#include <bluefruit.h>

// define the LED pins
const int LED_PINS[] = {LED_RED, LED_BLUE, LED_GREEN};
const int NUM_LEDS = sizeof(LED_PINS) / sizeof(LED_PINS[0]);
const char* IMU_SERVICE_UUID = "4401d1bb-95c6-461e-9579-cf6989cccddc";
const char* IMU_X_UUID = "7b53dc33-a8ca-41af-a269-2eb2bb432b40";
const char* IMU_Y_UUID = "7b53dc33-a8ca-41af-a269-2eb2bb432b50";
const char* IMU_Z_UUID = "7b53dc33-a8ca-41af-a269-2eb2bb432b60";

BLEService ble_imu_service(IMU_SERVICE_UUID);
BLECharacteristic imu_x(IMU_X_UUID, BLERead, 4 , true);
BLECharacteristic imu_y(IMU_Y_UUID, BLERead, 4 , true);
BLECharacteristic imu_z(IMU_Z_UUID, BLERead, 4 , true);

void initLEDs();
void initSerial();
void startAdv(void);
void initBLE();

void setup() 
{
  initSerial();
  initLEDs();
  initBLE();
  digitalWrite(LED_GREEN, LOW);
}

void loop() 
{
  imu_x.writeFloat(random(1000)/1000.0);
  imu_y.writeFloat(random(1000)/1000.0);
  imu_z.writeFloat(random(1000)/1000.0);
  delay(1000);
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
  imu_x.begin();
  imu_y.begin();
  imu_z.begin();
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

