//Juan Diego Mora
//10/30/2025
//Wireless Maraca Midi Controller

/*
some methods from CodeCell

Motion_AccelerometerRead(float &x, float &y, float &z) // Accelerometer (m/s²)
Motion_GyroRead(float &x, float &y, float &z) // Gyroscope (°/s)
Motion_MagnetometerRead(float &x, float &y, float &z) // Magnetometer (µT)
Motion_GravityRead(float &x, float &y, float &z) // Gravity vector
Motion_LinearAccRead(float &x, float &y, float &z) // Linear acceleration
Motion_RotationRead(float &roll, float &pitch, float &yaw) // Rotation (degrees)
Motion_RotationNoMagRead(float &roll, float &pitch, float &yaw) // Rotation w/o mag
Motion_RotationVectorRead(float &vec_r, float &vec_i, float &vec_j, float &vec_k) // Quaternion

 For CodeCell C3 (ESP32-C3):
  - Board: ESP32C3 Dev Module
  - USB CDC On Boot: Enabled   // required for Serial over USB
  - CPU Frequency: 160 MHz
  - Flash Size: 4 MB (32 Mb)
  - Partition Scheme: Minimal SPIFFS (1.9MB APP with OTA / 190KB SPIFFS)
  - Port: Select the COM port for your CodeCell C3

myCodeCell.PrintSensors(); //to print all enabled sensors to serial monitor

*/

//There's somthing wrong with the esp32 library that I can't remember... if things don't work go back to version 2.0.17
// Sometimes the native Arduino BLE-MIDI library won't work. In this case, just use the one I have forked in my repository.


/*
Some other notes on IMUs

Accelerometer : speed on 3 axis
Gyroscope : rotation on 3 axis
Magnetometer : strength of magnetic field on 3 axis

9 axis IMUs

*/

#include <CodeCell.h>
#include <BLEMIDI_Transport.h>
#include <hardware/BLEMIDI_ESP32.h>

CodeCell myCodeCell;
BLEMIDI_CREATE_INSTANCE("CODECELL_MIDI", MIDI)

//BLEMIDI_CREATE_DEFAULT_INSTANCE()


bool isConnected = false;
float x = 0.0;
float y = 0.0;
float z1 = 0.0;
float z2 = 0.0;
float threshold_movement = 20.0;
float threshold_stationary = 10.0;

void setup() {

  Serial.begin(115200);

  myCodeCell.Init(MOTION_LINEAR_ACC);

  BLEMIDI.setHandleConnected([]() {
    isConnected = true;
    Serial.println("Connected");
  });

  BLEMIDI.setHandleDisconnected([]() {
    isConnected = false;
    Serial.println("Disconnected");
  });

  MIDI.begin();
}

/*
TODO:

- LED control:

  - show Battery percentage

- Movement Control

  - 

*/

void loop() {
  if (myCodeCell.Run(10)) {
    //myCodeCell.PrintSensors();  // Print all enabled sensor values
    myCodeCell.Motion_LinearAccRead(x, y, z1);  // Linear acceleration

    if (isConnected) {
      if (z1 < (-threshold_stationary) && z2 > threshold_movement) {
        MIDI.sendNoteOn(60, 127, 1);
        Serial.print("DOWN");
        Serial.print(" z1 = ");
        Serial.print(z1);
        Serial.print(" z2 = ");
        Serial.println(z2);
      }
      if (z1 > threshold_stationary && z2 < (-threshold_movement)) {
        MIDI.sendNoteOn(62, 127, 1);
        Serial.print("UP");
        Serial.print(" z1 = ");
        Serial.print(z1);
        Serial.print(" z2 = ");
        Serial.println(z2);
      }
    }

    z2 = z1;  //UPDATE LAST STATE
  }
}
