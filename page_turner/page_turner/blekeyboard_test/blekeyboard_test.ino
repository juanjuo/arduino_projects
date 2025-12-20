#include <NimBLEDevice.h>
#include <NimBLEHIDDevice.h>

const int BUTTON_PIN = 1;

// --------------------
// Keyboard keycodes
// --------------------
#define KEY_NONE     0x00
#define KEY_PAGEDOWN 0x4E
// You can add more if needed:
// #define KEY_PAGEUP   0x4B
// #define KEY_ENTER    0x28
// #define KEY_RIGHT    0x4F
// #define KEY_LEFT     0x50
// #define KEY_DOWN     0x51
// #define KEY_UP       0x52

NimBLEHIDDevice* hid;
NimBLECharacteristic* inputReport;

class ServerCallbacks : public NimBLEServerCallbacks {
    void onConnect(NimBLEServer* server) override {
        Serial.println("Connected!");
    }
    void onDisconnect(NimBLEServer* server) override {
        Serial.println("Disconnected!");
        NimBLEDevice::startAdvertising();
    }
};

// -------------------------------------------------------------
// Simple HID Report Descriptor for a Standard Keyboard
// -------------------------------------------------------------
uint8_t reportMap[] = {
    USAGE_PAGE(1),      0x01,      // Generic Desktop
    USAGE(1),           0x06,      // Keyboard
    COLLECTION(1),      0x01,      // Application
        REPORT_ID(1),   0x01,
        USAGE_PAGE(1),  0x07,      // Keypad / Keyboard page
        USAGE_MINIMUM(1), 0xE0,
        USAGE_MAXIMUM(1), 0xE7,
        LOGICAL_MINIMUM(1), 0x00,
        LOGICAL_MAXIMUM(1), 0x01,
        REPORT_SIZE(1), 0x01,
        REPORT_COUNT(1), 0x08,
        INPUT(1),       0x02,      // Modifiers

        REPORT_SIZE(1), 0x08,
        REPORT_COUNT(1), 0x01,
        INPUT(1),       0x01,      // Reserved

        REPORT_SIZE(1), 0x08,
        REPORT_COUNT(1), 0x06,
        LOGICAL_MINIMUM(1), 0x00,
        LOGICAL_MAXIMUM(1), 0xE7,
        USAGE_MINIMUM(1), 0x00,
        USAGE_MAXIMUM(1), 0xE7,
        INPUT(1),       0x00,      // Keycodes
    END_COLLECTION(0)
};

// -------------------------------------------------------------
// Send a single key code (press + release)
// -------------------------------------------------------------
void sendKey(uint8_t keycode) {
    uint8_t report[8] = {0};  // modifiers, reserved, 6 key slots

    report[2] = keycode;  // put key in first slot
    inputReport->setValue(report, 8);
    inputReport->notify();
    delay(10);

    // Release
    memset(report, 0, 8);
    inputReport->setValue(report, 8);
    inputReport->notify();
}

void setup() {
    Serial.begin(115200);
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    NimBLEDevice::init("ESP32C3_Keyboard");
    NimBLEServer* server = NimBLEDevice::createServer();
    server->setCallbacks(new ServerCallbacks());

    hid = new NimBLEHIDDevice(server);
    inputReport = hid->inputReport(1);  // REPORT_ID = 1

    hid->manufacturer()->setValue("ESP32C3");
    hid->pnp(0x02, 0x045E, 0x0000, 0x0110);
    hid->hidInfo(0x00, 0x01);

    hid->setReportMap((uint8_t*)reportMap, sizeof(reportMap));
    hid->startServices();

    NimBLEAdvertising* ad = server->getAdvertising();
    ad->setAppearance(0x03C1);  // Keyboard appearance ID
    ad->addServiceUUID(hid->hidService()->getUUID());
    ad->start();

    Serial.println("Waiting for connection...");
}

bool lastState = HIGH;

void loop() {
    int reading = digitalRead(BUTTON_PIN);

    if (reading == LOW && lastState == HIGH) {
        Serial.println("PAGE DOWN");
        sendKey(KEY_PAGEDOWN);
    }

    lastState = reading;
}
