#include <cocotouch.h>

CocoTouch touch;

void setup() {
  if (!touch.begin()) {
    while (1) { ; }
  }
}

uint16_t lasttouched[12] = {0};

void loop() {
  for (uint8_t i = 0; i < 12; i++) {
    if (touch.getTouched(i) == 1 && lasttouched[i] == 0) {
      touch.playMIDINote(60 + i, 127, 1);
    }
    lasttouched[i] = touch.getTouched(i);
  }
}