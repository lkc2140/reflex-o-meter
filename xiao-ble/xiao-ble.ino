
// I want to create an ardunio ble application that sends 3 float values.  
// Please suggest improvements to the arduino code.  On the receiving side 
// a python script will write data to a file. Please help me with the python

// define the LED pins
const int LED_PINS[] = {LED_RED, LED_BLUE, LED_GREEN};
const int NUM_LEDS = sizeof(LED_PINS) / sizeof(LED_PINS[0]);
const char* IMU_SERVICE_UUID = "4401d1bb-95c6-461e-9579-cf6989cccddc";
const char* IMU_X_UUID = "7b53dc33-a8ca-41af-a269-2eb2bb432b40";
const char* IMU_Y_UUID = "7b53dc33-a8ca-41af-a269-2eb2bb432b50";
const char* IMU_Z_UUID = "7b53dc33-a8ca-41af-a269-2eb2bb432b60";

#include <bluefruit.h>

BLEDis ble_device_information_service;
BLEService ble_imu_service(IMU_SERVICE_UUID);
BLECharacteristic imu_x(IMU_X_UUID, BLEWrite, 4 , true);
BLECharacteristic imu_y(IMU_Y_UUID, BLEWrite, 4 , true);
BLECharacteristic imu_z(IMU_Z_UUID, BLEWrite, 4 , true);

bool hasKeyPressed = false;

void setup() 
{
  // Initialize the LED pins as outputs.
  for (int i = 0; i < NUM_LEDS; i++) {
    pinMode(LED_PINS[i], OUTPUT);
    digitalWrite(LED_PINS[i], HIGH);    // turn the LED off by making the voltage HIGH
  }
  // Turn on the blue led
  digitalWrite(LED_RED, LOW);  // HIGH is OFF, LOW is ON ... smh
  // Set serial port speed
  Serial.begin(115200);
  while ( !Serial ) delay(10);
  Serial.println("Starting...");
  Bluefruit.begin();
  Bluefruit.setName("Reflex-o-meter");
  Bluefruit.setTxPower(4);
  
  // Configure and Start Device Information Service
  ble_device_information_service.setManufacturer("Reflex-o-meter Inc.");
  ble_device_information_service.setModel("Prototype-C");
  ble_device_information_service.begin();

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
  Bluefruit.Advertising.addTxPower();
  Bluefruit.Advertising.addAppearance(BLE_APPEARANCE_UNKNOWN);
  
  // Include BLE HID service
  Bluefruit.Advertising.addService(ble_imu_service);

  // There is enough room for the dev name in the advertising packet
  Bluefruit.Advertising.addName();
  
  /* Start Advertising
   * - Enable auto advertising if disconnected
   * - Interval:  fast mode = 20 ms, slow mode = 152.5 ms
   * - Timeout for fast mode is 30 seconds
   * - Start(timeout) with timeout = 0 will advertise forever (until connected)
   * 
   * For recommended advertising interval
   * https://developer.apple.com/library/content/qa/qa1931/_index.html   
   */
  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setInterval(20, 40);    // in unit of 0.625 ms
  Bluefruit.Advertising.setFastTimeout(0);      // number of seconds in fast mode
  Bluefruit.Advertising.start(0);                // 0 = Don't stop advertising after n seconds
}

void loop() 
{
  imu_x.writeFloat(1.0);
  imu_y.writeFloat(2.0);
  imu_z.writeFloat(3.0);
  delay(1000);
}