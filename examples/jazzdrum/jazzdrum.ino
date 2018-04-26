#include <cocotouch.h>

CocoTouch touch;

void setup() {
  Serial.begin(115200);
  if (!touch.begin()) {
    while (1) { ; }
  }
}

#define VOL 80
#define ACOUSTIC_BASS_DRUM 35
#define ACOUSTIC_SNARE 38
#define ELECTRIC_SNARE 40
#define LOW_FOOR_TOM 41
#define CLOSED_HI_HAT 42
#define OPEN_HI_HAT 46
#define LOW_MID_TOM 47
#define HIGH_MID_TOM 48
#define CRASH_CYMBAL 49
#define HIGH_TOM 50
#define RIDE_CYMBAL 51
#define CHINESE_CYMBAL 52

uint8_t drums[] = {ACOUSTIC_BASS_DRUM, ACOUSTIC_SNARE, ELECTRIC_SNARE,
                   CLOSED_HI_HAT,      OPEN_HI_HAT,    LOW_FOOR_TOM,
                   LOW_MID_TOM,        HIGH_MID_TOM,   HIGH_TOM,
                   RIDE_CYMBAL,        CRASH_CYMBAL,   CHINESE_CYMBAL};

uint16_t lasttouched[12] = {0};

void loop() {
  for (uint8_t i = 0; i < 12; i++) {
    if (touch.getTouched(i) == 1 && lasttouched[i] == 0) {
      touch.playMIDINote(drums[i], VOL);
    }
    lasttouched[i] = touch.getTouched(i);
  }
}