Hardware:
---------
 - BLE nano http://redbearlab.com/blenano/

Requirements:
-------------
 - Arduino 1.6.9
 - Add boards as described here https://github.com/redbearlab/nRF51822-Arduino/s


Service:
  BLEService              sensorService2        = BLEService("19b10000e8f2537e4f6cd104768a1214");

MAC: F0:D0:C5:95:90:14

Good links
https://devzone.nordicsemi.com/tutorials/8/


Now I get a null value from the service.
========================================
As described in https://stackoverflow.com/questions/45736684/bluetoothgattservice-getcharacteristic-returns-null-on-android-6
It worked on Android 6 after we changed the service UUID's from:

5765536d-0000-1000-8000-00805f9b34fb
5765536e-0000-1000-8000-00805f9b34fb
To:

3032454c-426b-7261-5074-72616d536557
3031454c-426b-7261-5074-72616d536557
It also worked when we reduced the UUID's size from 32bits to 16bits, this means, the first four digits being zero 0000XXXX.
