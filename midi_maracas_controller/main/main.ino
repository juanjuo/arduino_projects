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

myCodeCell.PrintSensors(); //to print all enabled sensors to serial monitor

*/

#include <CodeCell.h>
#include <BLEMIDI_Transport.h>
#include <hardware/BLEMIDI_ESP32.h>

CodeCell myCodeCell;

BLEMIDI_CREATE_DEFAULT_INSTANCE()

void setup() {

  myCodeCell.Init(MOTION_ACCELEROMETER); 
  MIDI.begin();
  
}

void loop() {
  MIDI.read();

  //



  if (myCodeCell.Run(100)) {  
    
  }
}
