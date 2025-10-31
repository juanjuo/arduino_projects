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
