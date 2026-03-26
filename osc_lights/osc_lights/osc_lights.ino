// #include <ArduinoOSCWiFi.h>

// // WiFi stuff
// const char* ssid = "your-ssid";
// const char* pwd = "your-password";
// const IPAddress ip(192, 168, 1, 201);
// const IPAddress gateway(192, 168, 1, 1);
// const IPAddress subnet(255, 255, 255, 0);

// // for ArduinoOSC
// const char* host = "192.168.1.200";
// const int recv_port = 54321;
// const int bind_port = 54345;
// const int send_port = 55555;
// const int publish_port = 54445;

// int i;
// float f;
// String s;

const int ledPin = 13;
const int pwmFreq = 1000;     // 1 kHz
const int pwmResolution = 8;  // 8-bit PWM (0–255)

// void onOscReceived(const OscMessage& m) {
//     Serial.print(m.remoteIP());
//     Serial.print(" ");
//     Serial.print(m.remotePort());
//     Serial.print(" ");
//     Serial.print(m.size());
//     Serial.print(" ");
//     Serial.print(m.address());
//     Serial.print(" ");
//     Serial.print(m.arg<int>(0));
//     Serial.print(" ");
//     Serial.print(m.arg<float>(1));
//     Serial.print(" ");
//     Serial.print(m.arg<String>(2));
//     Serial.println();
// }

void setup() {
  // delay(2000);

  ledcAttach(ledPin, pwmFreq, pwmResolution);
  Serial.begin(115200);
}

void loop() {

  for (int value = 0; value <= 255; value++) {
    ledcWrite(ledPin, value);
    Serial.println(value);
    delay(2);
  }

  for (int value = 255; value >= 0; value--) {
    ledcWrite(ledPin, value);
    Serial.println(value);
    delay(2);
  }

}



