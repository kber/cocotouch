#include <cocotouch.h>

CocoTouch touch;

#define TEMPO 300
#define VOL 80

void simple() {
  touch.playMIDINote(42, VOL); // Closed Hi-hat
  touch.playMIDINote(36, VOL); // Bass Drum
  delay(TEMPO);
  touch.playMIDINote(42, VOL); // Closed Hi-hat
  delay(TEMPO);
  touch.playMIDINote(42, VOL); // Closed Hi-hat
  touch.playMIDINote(38, VOL); // Acoustic Snare Drum
  delay(TEMPO);
  touch.playMIDINote(42, VOL); // Closed Hi-hat
  delay(TEMPO);
}

void unyielding() {
  touch.playMIDINote(42, VOL); // Closed Hi-hat
  touch.playMIDINote(36, VOL); // Bass Drum
  delay(200);
  touch.playMIDINote(42, VOL); // Closed Hi-hat
  delay(200);

  touch.playMIDINote(42, VOL); // Closed Hi-hat
  touch.playMIDINote(36, VOL); // Bass Drum
  delay(200);
  touch.playMIDINote(42, VOL); // Closed Hi-hat
  delay(200);
  touch.playMIDINote(42, VOL); // Closed Hi-hat
  touch.playMIDINote(38, VOL); // Acoustic Snare Drum
  delay(600);
  touch.playMIDINote(42, VOL); // Closed Hi-hat
  delay(200);

  ////////////
  touch.playMIDINote(42, VOL); // Closed Hi-hat
  touch.playMIDINote(36, VOL); // Bass Drum
  delay(200);
  touch.playMIDINote(42, VOL); // Closed Hi-hat
  delay(200);

  touch.playMIDINote(42, VOL); // Closed Hi-hat
  touch.playMIDINote(38, VOL); // High Mid Tom
  delay(200);
  touch.playMIDINote(42, VOL); // Closed Hi-hat
  delay(200);
  touch.playMIDINote(42, VOL); // Closed Hi-hat
  touch.playMIDINote(38, VOL); // Acoustic Snare Drum
  delay(600);
  touch.playMIDINote(42, VOL); // Closed Hi-hat
  delay(200);
}

void setup() {
  Serial.begin(115200);
  if (!touch.begin()) {
    while (1) { ; }
  }
}

void loop() { simple(); }