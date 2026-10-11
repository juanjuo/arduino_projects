// Wireless midi controller v2
// Juan Diego Mora Rubio
// 10/10/26

const int muxS0 = 2;
const int muxS1 = 4;
const int muxS2 = 16;
const int muxS3 = 17;
const int muxSIG = 5;

const int NUM_BUTTONS = 4;

bool buttonState[NUM_BUTTONS];
bool previousState[NUM_BUTTONS];

void selectChannel(int channel) {
    digitalWrite(muxS0, (channel >> 0) & 1);
    digitalWrite(muxS1, (channel >> 1) & 1);
    digitalWrite(muxS2, (channel >> 2) & 1);
    digitalWrite(muxS3, (channel >> 3) & 1);
}

void setup() {
    Serial.begin(115200);

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
}

void loop() {
    for (int i = 0; i < NUM_BUTTONS; i++) {
        selectChannel(i);

        // Allow the signal to settle after switching channels.
        delayMicroseconds(5);

        buttonState[i] = digitalRead(muxSIG);

        if (buttonState[i] != previousState[i]) {
            if (buttonState[i] == LOW) {
                Serial.printf("Button %d pressed\n", i);
            } else {
                Serial.printf("Button %d released\n", i);
            }

            previousState[i] = buttonState[i];
        }
    }
}
