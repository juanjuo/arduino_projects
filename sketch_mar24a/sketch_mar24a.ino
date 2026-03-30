#include <WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>

// const char* ssid = "juan_phone";
// const char* password = "123456789";

char ssid[] = "Joan and Jeff (appt)";
char password[] = "easycanoe613";

WiFiUDP udp;
const int localPort = 57120;

void setup() {
  Serial.begin(115200);

  //WiFi.setTxPower(WIFI_POWER_8_5dBm);

  //Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");
  Serial.print("ESP32 IP: ");
  Serial.println(WiFi.localIP());

  // Start UDP listener
  udp.begin(localPort);
  Serial.printf("Listening for OSC on port %d\n", localPort);
}

void loop() {
  OSCMessage msg;
  int packetSize = udp.parsePacket();

  if (packetSize) {
    // Fill OSC message with incoming bytes
    while (packetSize--) {
      msg.fill(udp.read());
    }

    if (!msg.hasError()) {
      printOSCMessage(msg);
    } else {
      Serial.println("OSC message error");
    }
  }
}

void printOSCMessage(OSCMessage& msg) {
  // Print address
  char address[64];
  msg.getAddress(address);
  Serial.print("Address: ");
  Serial.println(address);

  // Print arguments
  Serial.print("Args: ");

  for (int i = 0; i < msg.size(); i++) {
    if (msg.isInt(i)) {
      Serial.print(msg.getInt(i));
    } else if (msg.isFloat(i)) {
      Serial.print(msg.getFloat(i));
    } else if (msg.isString(i)) {
      char str[64];
      msg.getString(i, str, 64);
      Serial.print(str);
    } else {
      Serial.print("[unknown]");
    }

    if (i < msg.size() - 1) {
      Serial.print(", ");
    }
  }

  Serial.println();
  Serial.println("--------------------");
}