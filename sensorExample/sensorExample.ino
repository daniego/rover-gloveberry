

/*
  Sensor reading

  Uses Sandeep Mistry's BLE Peripheral library: https://github.com/sandeepmistry/arduino-BLEPeripheral/
  to read data from a sensor.

  created 6 Feb 2015
  by Maria Paula Saba
*/


// Import libraries (BLEPeripheral depends on SPI)
#include <SPI.h>
#include <BLEPeripheral.h>
//#include <BLE_API.h>
// define pins (varies per shield/board, UNUSED for nRF51822)
#define BLE_REQ   10
#define BLE_RDY   2
#define BLE_RST   9

//define sensor pin
//#define SENSOR_PIN 5
#define SENSOR_PIN A3
#define SENSOR_PIN2 A4
//variable for a timer
long previousMillis = 0;

 // interval at which we change (send) data (milliseconds)
long interval = 100;

// create peripheral instance, see pinouts above
BLEPeripheral           blePeripheral         = BLEPeripheral(BLE_REQ, BLE_RDY, BLE_RST);

// service for pin A3
//Nordic LED Button Service
//BLEService              sensorService         = BLEService("00001523-1212-efde-1523-785feabcd123");
BLEService              sensorService         = BLEService("33332a58-0000-1000-8000-00805f9b34fb");

// service for pin A4
//BLEService              sensorService2        = BLEService("3032454c-426b-7261-5074-72616d536558");

// create switch characteristic for pin A3
BLECharCharacteristic   sensorCharacteristic  = BLECharCharacteristic("33332a58-0000-1000-8000-00805f9b34fb", BLERead | BLENotify);

// create switch characteristic for pin A4
BLECharCharacteristic   sensorCharacteristic2 = BLECharCharacteristic("33332a58-0000-1000-8000-00805f9b34fc", BLERead | BLENotify);

void setup() {
//  Serial.begin(115200);
  Serial.begin(9600);
  #if defined (__AVR_ATmega32U4__)
    delay(5000);  //5 seconds delay for enabling to see the start up comments on the serial board
  #endif

   //set sensor pin
   pinMode(SENSOR_PIN, INPUT);
   pinMode(SENSOR_PIN2, INPUT);

  // set advertised local name and service UUID
  blePeripheral.setLocalName("Sensor");
  blePeripheral.setAdvertisedServiceUuid(sensorService.uuid());
//  blePeripheral.setAdvertisedServiceUuid(sensorService2.uuid());

  // add service and characteristic
  blePeripheral.addAttribute(sensorService);
//  blePeripheral.addAttribute(sensorService2);
  blePeripheral.addAttribute(sensorCharacteristic);
  blePeripheral.addAttribute(sensorCharacteristic2);

  // assign event handlers for connected, disconnected to peripheral
  blePeripheral.setEventHandler(BLEConnected, blePeripheralConnectHandler);
  blePeripheral.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);

  // assign event handlers for characteristic
  sensorCharacteristic.setEventHandler(BLESubscribed,   characteristicSubscribed);
  sensorCharacteristic.setEventHandler(BLEUnsubscribed, characteristicUnsubscribed);

  sensorCharacteristic2.setEventHandler(BLESubscribed,   characteristicSubscribed);
  sensorCharacteristic2.setEventHandler(BLEUnsubscribed, characteristicUnsubscribed);

  // begin initialization
  blePeripheral.begin();

  Serial.println(F("BLE Sensor Peripheral"));
}

void loop() {
  // poll peripheral - this function will start the peripheral and handle the callbacks
  blePeripheral.poll();

  //timer function
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > interval) {
    // save the last time
    previousMillis = currentMillis;

    // read the analog input
    int analogValue = analogRead(SENSOR_PIN);
    int analogValue2 = analogRead(SENSOR_PIN2);
    //save it in the characteristic
    sensorCharacteristic.setValue(analogValue);
    
    sensorCharacteristic2.setValue(analogValue2);
    
    Serial.println("Sensor 1");
//946
    Serial.println(analogValue);
  
    // print it out in many formats:
    Serial.println(analogValue);       // print as an ASCII-encoded decimal
    Serial.print("DEC "); Serial.println(analogValue, DEC);  // print as an ASCII-encoded decimal
    Serial.print("HEX "); Serial.println(analogValue, HEX);  // print as an ASCII-encoded hexadecimal
    Serial.print("OCT "); Serial.println(analogValue, OCT);  // print as an ASCII-encoded octal
    Serial.print("BIN "); Serial.println(analogValue, BIN);  // print as an ASCII-encoded binary

    Serial.println("Sensor 2");
    Serial.println("=================================================================================");

 }

}

//callback functions for connect, disconnect and written characteristic are described below:
void blePeripheralConnectHandler(BLECentral& central) {
  // central connected event handler
  Serial.print(F("Connected event, central: "));
  Serial.println(central.address());

}

void blePeripheralDisconnectHandler(BLECentral& central) {
  // central disconnected event handler
  Serial.print(F("Disconnected event, central: "));
  Serial.println(central.address());
}


void characteristicSubscribed(BLECentral& central, BLECharacteristic& characteristic) {
  // characteristic subscribed event handler
  Serial.println(F("Characteristic event, subscribed"));
}

void characteristicUnsubscribed(BLECentral& central, BLECharacteristic& characteristic) {
  // characteristic unsubscribed event handler
  Serial.println(F("Characteristic event, unsubscribed"));
}
