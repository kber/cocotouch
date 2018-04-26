#include <cocotouch.h>

uint8_t touchPinStates[12] = {0};
uint8_t thresholdValue = 100;
CocoTouch touch;

void setup() {
  if (!touch.begin()) {
    while (1) { ; }
  }
  Serial.begin(9600);
}

void loop() {
  for (uint8_t pinIndex = 0; pinIndex <= 11; pinIndex++) {
    if (touchPinStates[pinIndex] == 0 &&
        touch.getTouchPinValue(pinIndex) <= thresholdValue) {
      touchPinStates[pinIndex] = 1;
      touch.sendMessage(String("touched:") + String(pinIndex));
    } else if (touchPinStates[pinIndex] == 1 &&
               touch.getTouchPinValue(pinIndex) > thresholdValue) {
      touchPinStates[pinIndex] = 0;
      touch.sendMessage(String("released:") + String(pinIndex));
    }
  }
}