/*
  BLE_Peripheral.ino

  This program uses the ArduinoBLE library to set-up an Arduino Nano 33 BLE 
  as a peripheral device and specifies a service and a characteristic. Depending 
  of the value of the specified characteristic, an on-board LED gets on. 

  The circuit:
  - Arduino Nano 33 BLE. 

  This example code is in the public domain.
*/

#include <ArduinoBLE.h>
      
enum {
  GESTURE_NONE  = -1,
  GESTURE_UP    = 0,
  GESTURE_DOWN  = 1,
  GESTURE_LEFT  = 2,
  GESTURE_RIGHT = 3
};

const char* deviceServiceUuid = "0000fff0-0000-1000-8000-00805f9b34fb";
const char* deviceServiceCharacteristicUuid = "0000fff1-0000-1000-8000-00805f9b34fb";

int gesture = -1;

BLEService gestureService(deviceServiceUuid); 
BLEByteCharacteristic gestureCharacteristic(deviceServiceCharacteristicUuid, BLERead | BLEWrite | BLENotify);


void setup() {
  Serial.begin(9600);
  while (!Serial);  
  
  // pinMode(LEDR, OUTPUT);
  // pinMode(LEDG, OUTPUT);
  // pinMode(LEDB, OUTPUT);
  // pinMode(LED_BUILTIN, OUTPUT);
  
  // digitalWrite(LEDR, HIGH);
  // digitalWrite(LEDG, HIGH);
  // digitalWrite(LEDB, HIGH);
  // digitalWrite(LED_BUILTIN, LOW);

  
  if (!BLE.begin()) {
    Serial.println("- Starting BLE module failed!");
    while (1);
  }

  BLE.setDeviceName( "Arduino Nano 33 BLE" );
  BLE.setLocalName( "Arduino Nano 33 BLE" );

    // Build scan response data packet
  BLEAdvertisingData scanData;
  // Set parameters for scan response packet
  scanData.setLocalName("Arduino Nano 33 BLE");
  // Copy set parameters in the actual scan response packet
  BLE.setScanResponseData(scanData);


  BLE.setAdvertisedService(gestureService);
  gestureService.addCharacteristic(gestureCharacteristic);
  BLE.addService(gestureService);
  gestureCharacteristic.writeValue(-1);
  BLE.advertise();

  Serial.println("Nano 33 BLE (Peripheral Device)");
  Serial.println(" ");
}

void loop() {
  BLEDevice central = BLE.central();
  //Serial.println("- Discovering central device...");
  delay(500);

  if (central) {
    Serial.println("* Connected to central device!");
    Serial.print("* Device MAC address: ");
    Serial.println(central.address());
    Serial.println(" ");

    while (central.connected()) {
      if (gestureCharacteristic.written()) {
          gesture = gestureCharacteristic.value();
          Serial.println("Value has been written: ");
          Serial.println(gesture);
          writeGesture(gesture);
       }

      if (gestureCharacteristic.subscribed()) {
        // set a new value , that well be pushed to subscribed BLE devices
        Serial.println("writeValue");
        gestureCharacteristic.writeValue(0x1);
      }
    }
    
    Serial.println("* Disconnected to central device!");
  }
}

void writeGesture(int gesture) {
  Serial.println("- Characteristic <gesture_type> has changed!");

  Serial.println(gesture);
  
  //  switch (gesture) {
  //     case GESTURE_UP:
  //       Serial.println("* Actual value: UP (red LED on)");
  //       Serial.println(" ");
  //       digitalWrite(LEDR, LOW);
  //       digitalWrite(LEDG, HIGH);
  //       digitalWrite(LEDB, HIGH);
  //       digitalWrite(LED_BUILTIN, LOW);
  //       break;
  //     case GESTURE_DOWN:
  //       Serial.println("* Actual value: DOWN (green LED on)");
  //       Serial.println(" ");
  //       digitalWrite(LEDR, HIGH);
  //       digitalWrite(LEDG, LOW);
  //       digitalWrite(LEDB, HIGH);
  //       digitalWrite(LED_BUILTIN, LOW);
  //       break;
  //     case GESTURE_LEFT:
  //       Serial.println("* Actual value: LEFT (blue LED on)");
  //       Serial.println(" ");
  //       digitalWrite(LEDR, HIGH);
  //       digitalWrite(LEDG, HIGH);
  //       digitalWrite(LEDB, LOW);
  //       digitalWrite(LED_BUILTIN, LOW);
  //       break;
  //     case GESTURE_RIGHT:
  //       Serial.println("* Actual value: RIGHT (built-in LED on)");
  //       Serial.println(" ");
  //       digitalWrite(LEDR, HIGH);
  //       digitalWrite(LEDG, HIGH);
  //       digitalWrite(LEDB, HIGH);
  //       digitalWrite(LED_BUILTIN, HIGH);
  //       break;
  //     default:
  //       digitalWrite(LEDR, HIGH);
  //       digitalWrite(LEDG, HIGH);
  //       digitalWrite(LEDB, HIGH);
  //       digitalWrite(LED_BUILTIN, LOW);
  //       break;
  //   }      
}