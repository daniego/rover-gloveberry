

/*
  Sensor reading

  Uses BLE Peripheral library: https://github.com/daniego/reover-gloveberry/
  to read data from anologue sensors.

  created 21 Jan 2018
  by Daniel Floris
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
#define SENSOR_PIN  A3
#define SENSOR_PIN2 A4

//variable for a timer
long previousMillis = 0;

 // interval at which we change (send) data (milliseconds)
long interval = 500;

// create peripheral instance, see pinouts above
BLEPeripheral           blePeripheral         = BLEPeripheral(BLE_REQ, BLE_RDY, BLE_RST);

// service
BLEService              sensorService         = BLEService("33332a58-0000-1000-8000-00805f9b34ff");

// create switch characteristic for pin A3
BLECharCharacteristic   sensorCharacteristic  = BLECharCharacteristic("33332a58-0000-1000-8000-00805f9b34aa", BLERead | BLENotify);

// create switch characteristic for pin A4
BLECharCharacteristic   sensorCharacteristic2 = BLECharCharacteristic("33332a58-0000-1000-8000-00805f9b34bb", BLERead | BLENotify);

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

  // add service and characteristic
  blePeripheral.addAttribute(sensorService);
  
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

    Serial.print("Sensor 1: ");Serial.println(analogValue); 
    Serial.print("Sensor 2: ");Serial.println(analogValue2); 
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
