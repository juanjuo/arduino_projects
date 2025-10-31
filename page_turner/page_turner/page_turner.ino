#include <BleKeyboard.h>
//#include "esp_sleep.h"
//#include "esp_wifi.h"
//#include "driver/adc.h"

BleKeyboard bleKeyboard;

const int rightButton = 1;
bool rightHasBeenPressed = false;

// debounce state
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;  // milliseconds
bool lastButtonState = HIGH;
bool currentButtonState = HIGH;

void setup() 
{
  Serial.begin(115200);
  Serial.println("Starting BLE work!");
  // Optional: Reduce power
  //esp_wifi_stop();      // If Wi-Fi accidentally started
  //esp_wifi_deinit();
  //adc_power_off();      // If no analog sensors
  bleKeyboard.begin();

  pinMode(rightButton, INPUT_PULLUP);  // make sure you have a pull-up/down resistor externally

  // esp_sleep_enable_gpio_wakeup();  // globally allow GPIO wakeup
  // gpio_wakeup_enable(GPIO_NUM_32, GPIO_INTR_LOW_LEVEL);  // trigger on LOW
}

void loop() 
{
  if (bleKeyboard.isConnected()) 
  {
    int reading = digitalRead(rightButton);

    if (reading != lastButtonState) {
      lastDebounceTime = millis();  // reset debounce timer
    }

    if ((millis() - lastDebounceTime) > debounceDelay) 
    {
      if (reading == LOW && !rightHasBeenPressed) 
      {
        Serial.println("right button pressed");
        bleKeyboard.write(KEY_PAGE_DOWN);
        rightHasBeenPressed = true;
      } else if (reading == HIGH && rightHasBeenPressed) 
        {
        Serial.println("RESET");
        rightHasBeenPressed = false;
        }
    }

    lastButtonState = reading;
  }

  // Sleep when idle
  // if (!rightHasBeenPressed && bleKeyboard.isConnected()) 
  // {
  //     Serial.println("Entering light sleep...");
  //     esp_light_sleep_start();  // CPU pauses until button is pressed
  //     Serial.println("Woke up!");
  // }
}