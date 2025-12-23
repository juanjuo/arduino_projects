#include <BLEMIDI_Transport.h>       //for BLEMIDI compatibility
#include <hardware/BLEMIDI_ESP32.h>  // include all BLEMIDI library code for ESP32

BLEMIDI_CREATE_INSTANCE("BLE_MIDI_KEYBOARD", MIDI)

const int NUM_KEYS = 12;

int keyPins[NUM_KEYS] = { 0, 1, 2, 3, 4, 5, 6, 7, 10, 21};

int keyLastState[NUM_KEYS] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

int notes[NUM_KEYS] = { 60, 61, 62, 63, 64, 65, 66, 67, 68, 70};

int keyState = LOW;

bool isConnected = false;


void setup() {
  //Serial.begin(115200);

  BLEMIDI.setHandleConnected([]() {
    isConnected = true;
    //Serial.println("Connected");
  });

  BLEMIDI.setHandleDisconnected([]() {
    isConnected = false;
    //Serial.println("Disconnected");
  });

  for (int i = 0; i < NUM_KEYS; i++) {
    pinMode(keyPins[i], INPUT_PULLUP);
  }

  MIDI.begin();
}

void loop() {
  if (isConnected) {
    for (int i = 0; i < NUM_KEYS; i++) {
      //keyLastState[i] = keyState;
      keyState = digitalRead(keyPins[i]);

      if (keyState == LOW && keyLastState[i] == 0) {
        //Serial.println("Button Pressed");
        MIDI.sendNoteOn(notes[i], 127, 1);
        keyLastState[i] = 1;
      } else if (keyState == HIGH && keyLastState[i] == 1) {
        MIDI.sendNoteOff(notes[i], 127, 1);
        //Serial.println("RESET");
        keyLastState[i] = 0;
      }
    }
  }
}
