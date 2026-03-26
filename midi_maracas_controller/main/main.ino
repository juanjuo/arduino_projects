//Juan Diego Mora
//10/30/2025
//Wireless Maraca Midi Controller

/*
some methods from CodeCell

Motion_AccelerometerRead(float &x, float &y, float &z) // Accelerometer (m/s²) with respect to gravity
Motion_GyroRead(float &x, float &y, float &z) // Gyroscope (°/s)
Motion_MagnetometerRead(float &x, float &y, float &z) // Magnetometer (µT)
Motion_GravityRead(float &x, float &y, float &z) // Gravity vector
Motion_LinearAccRead(float &x, float &y, float &z) // Linear acceleration
Motion_RotationRead(float &roll, float &pitch, float &yaw) // Rotation (degrees)
Motion_RotationNoMagRead(float &roll, float &pitch, float &yaw) // Rotation w/o mag
Motion_RotationVectorRead(float &vec_r, float &vec_i, float &vec_j, float &vec_k) // Quaternion

MOTION_ACCELEROMETER // 3-axis acceleration
MOTION_GYRO // 3-axis angular velocity
MOTION_MAGNETOMETER // 3-axis magnetic field
MOTION_LINEAR_ACC // Linear acceleration
MOTION_GRAVITY // Gravity vector
MOTION_ROTATION // Roll, pitch, yaw (with mag)
MOTION_ROTATION_NO_MAG // Roll, pitch, yaw (no mag)
MOTION_STEP_COUNTER // Step count
MOTION_STATE // On table / Stationary / Stable / In motion
MOTION_TAP_DETECTOR // Tap detection
MOTION_ACTIVITY // Activity classification

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
//#include <BLEMIDI_Transport.h>
//#include <hardware/BLEMIDI_ESP32.h>

#include <WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>

CodeCell myCodeCell;

//FOR BLUETOOTH MIDI ....
//BLEMIDI_CREATE_INSTANCE("CODECELL_MIDI", MIDI)
//BLEMIDI_CREATE_DEFAULT_INSTANCE()

// FOR OSC
// char ssid[] = "Joan and Jeff";           // your network SSID (name)
// char pass[] = "easycanoe613";            // your network password
char ssid[] = "juan_phone";           // your network SSID (name)
char pass[] = "123456789";            // your network password
WiFiUDP Udp;                             // A UDP instance to let us send and receive packets over UDP
const IPAddress outIp(192, 168, 5, 26);  // remote IP of your computer
const unsigned int outPort = 57120;      // remote port to receive OSC
const unsigned int localPort = 57120;    // local port to listen for OSC packets (actually not used for sending)

//
float x = 0.0;
float y = 0.0;
float current_z = 0.0;
float negative_threshold = -2.0;
float positive_threshold = 2.0;

float filtered_z = 0.0;
float velocity = 0.0;
float alpha = 0.5;  //for low-pass filter

float velocity_decay = 0.995;  //drift compensation

int sampling_rate = 100; // for both CodeCell and Velocity

enum State {
  IDLE,
  DOWN
};

State state = IDLE;

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Starting UDP");
  Udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(localPort);

  myCodeCell.Init(MOTION_LINEAR_ACC);

  // BLEMIDI.setHandleConnected([]() {
  //   isConnected = true;
  //   Serial.println("Connected");
  // });

  // BLEMIDI.setHandleDisconnected([]() {
  //   isConnected = false;
  //   Serial.println("Disconnected");
  // });

  //MIDI.begin();
}

/*
TODO:

- LED control:

  - show Battery percentage

- Movement Control

  - 

*/

void loop() {
  if (myCodeCell.Run(sampling_rate)) {
    //myCodeCell.PrintSensors();  // Print all enabled sensor values
    myCodeCell.Motion_LinearAccRead(x, y, current_z);  // Linear acceleration

    // if(current_z > last_z || current_z < -last_z){
    //   if (i == max_index){
    //     if (current_z > 0){
    //       Serial.println("DOWN");
    //     }
    //     else {
    //       Serial.println("UP");
    //     }
    //     i = 0;
    //     last_z = threshold_stationary;
    //   }
    //   else{
    //     last_z = current_z;
    //     i++;
    //   }
    // }

    filtered_z = alpha * current_z + (1 - alpha) * filtered_z;

    velocity += filtered_z * 0.02;

    velocity *= velocity_decay;  // small decay because of drift

    // OSCMessage msg("/test");
    //   msg.add(current_z);
    //   Udp.beginPacket(outIp, outPort);
    //   msg.send(Udp);
    //   Udp.endPacket();
    //   msg.empty();

    Serial.println(velocity);

    switch (state) {

      case IDLE:
        if (velocity < negative_threshold) {
          state = DOWN;
        }
        break;

      case DOWN:
        if (velocity > positive_threshold) {
          Serial.println("DOWN");
          OSCMessage msg("/test");
          msg.add("DOWN");
          Udp.beginPacket(outIp, outPort);
          msg.send(Udp);
          Udp.endPacket();
          msg.empty();
          state = IDLE;
          velocity = 0;  // Reset after trigger
        }
        break;
    }

    // if (current_z < (-threshold_stationary) && last_z > threshold_stationary) {
    //   //MIDI.sendNoteOn(60, 127, 1);
    //   Serial.print("DOWN");
    //   OSCMessage msg("/test");
    //   msg.add("DOWN");
    //   Udp.beginPacket(outIp, outPort);
    //   msg.send(Udp);
    //   Udp.endPacket();
    //   msg.empty();
    //   Serial.print(" z1 = ");
    //   Serial.print(current_z);
    //   Serial.print(" z2 = ");
    //   Serial.println(last_z);
    // }
    // if (current_z > threshold_stationary && last_z < (-threshold_stationary)) {
    //   //MIDI.sendNoteOn(62, 127, 1);
    //   Serial.print("UP");
    //   OSCMessage msg("/test");
    //   msg.add("UP");
    //   Udp.beginPacket(outIp, outPort);
    //   msg.send(Udp);
    //   Udp.endPacket();
    //   msg.empty();
    //   Serial.print(" z1 = ");
    //   Serial.print(current_z);
    //   Serial.print(" z2 = ");
    //   Serial.println(last_z);
    // }

    //last_z = current_z;  //UPDATE LAST STATE
  }
}

// void sendOSCMessage(char[] type, char[] message) {
//   OSCMessage msg(type);
//   msg.add(message);
//   Udp.beginPacket(outIp, outPort);
//   msg.send(Udp);
//   Udp.endPacket();
//   msg.empty();
// }
