// Wireless midi controller v2
// Juan Diego Mora Rubio
// 10/10/26

#include <BLEMIDI_Transport.h>       //for BLEMIDI compatibility
#include <hardware/BLEMIDI_ESP32.h>  // include all BLEMIDI library code for ESP32

BLEMIDI_CREATE_INSTANCE("BLE_MIDI_KEYBOARD", MIDI)

const int muxS0 = 4;
const int muxS1 = 16;
const int muxS2 = 17;
const int muxS3 = 5;
const int muxSIG = 18;

const int onBoardLED = 2;

const int NUM_BUTTONS = 12;

bool isConnected = false;

bool buttonState[NUM_BUTTONS];
bool previousState[NUM_BUTTONS];
int notes[NUM_BUTTONS] = { 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71 };

void selectChannel(int channel) {
  digitalWrite(muxS0, (channel >> 0) & 1);
  digitalWrite(muxS1, (channel >> 1) & 1);
  digitalWrite(muxS2, (channel >> 2) & 1);
  digitalWrite(muxS3, (channel >> 3) & 1);
}

void setup() {
  Serial.begin(115200);

  pinMode(onBoardLED, OUTPUT);

  BLEMIDI.setHandleConnected([]() {
    isConnected = true;
    Serial.println("Connected");
    digitalWrite(onBoardLED, HIGH);
  });

  BLEMIDI.setHandleDisconnected([]() {
    isConnected = false;
    Serial.println("Disconnected");
  });

  pinMode(muxS0, OUTPUT);
  pinMode(muxS1, OUTPUT);
  pinMode(muxS2, OUTPUT);
  pinMode(muxS3, OUTPUT);

  pinMode(muxSIG, INPUT_PULLUP);

  // Initialize button states as released.
  for (int i = 0; i < NUM_BUTTONS; i++) {
    buttonState[i] = HIGH;
    previousState[i] = HIGH;
  }
  MIDI.begin();
}

void loop() {
  if (isConnected) {
    for (int i = 0; i < NUM_BUTTONS; i++) {
      selectChannel(i);

      // Allow the signal to settle after switching channels.
      delayMicroseconds(5);

      buttonState[i] = digitalRead(muxSIG);

      if (buttonState[i] != previousState[i]) {
        if (buttonState[i] == LOW) {
          MIDI.sendNoteOn(notes[i], 127, 1);
          Serial.printf("Button %d pressed\n", i);
        } else {
          MIDI.sendNoteOff(notes[i], 127, 1);
          Serial.printf("Button %d released\n", i);
        }

        previousState[i] = buttonState[i];
      }
    }
  } else {
    digitalWrite(onBoardLED, LOW);
  }
}