/**
BSD 3-Clause License

Copyright (c) 2018, cocoet
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of the copyright holder nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <Adafruit_MPR121.h>
#include <MusicPlayer.h>
#include <SoftwareSerial.h>

#include "cocotouch.h"

#define D5 5
#define D6 6
#define EOD 0
#define VOL_MAX 0xFE
#define VOL_MIN 0x00

Adafruit_MPR121 cap;
MusicPlayer player;
SoftwareSerial bleSerial(D5, D6); // RX, TX

bool CocoTouch::begin() {
  return this->beginMIDI() && this->beginTouchSensor() && this->beginBLE();
}

bool CocoTouch::beginBLE(long speed = BLE_SPEED) {
  bleSerial.begin(speed);
  return TRUE;
}

bool CocoTouch::beginAudio() {
  player.begin();

  return TRUE;
}

bool CocoTouch::beginMIDI() {
  player.beginMidi();
  this->setMIDIBank(MIDI_BANK_GM2);
  this->setMIDIProgram(1, 0);

  return TRUE;
}

void CocoTouch::setAudioMode(uint8_t mode = AUDIO_MODE_MIDI) {
  if (mode == AUDIO_MODE_FILE) {
    this->beginAudio();
  } else if (mode == AUDIO_MODE_MIDI) {
    this->beginMIDI();
  }
}

bool CocoTouch::beginTouchSensor(uint8_t i2cAddress = I2CADDR) {
  cap = Adafruit_MPR121();
  if (!cap.begin(i2cAddress)) {
    Serial.println("MPR121 not found, check wiring?");
    while (TRUE)
      ;
  }
  return TRUE;
}

void CocoTouch::playAudioFile(const char *name) { player.play(name); }
void CocoTouch::playAudioFiles() { player.play(); }

uint8_t Clamp(uint8_t value, uint8_t min, uint8_t max) {
  if (value > max) return max;
  if (value < min) return min;
  return value;
}

void CocoTouch::setVolume(uint8_t vol) { this->setVolume(vol, vol); }

void CocoTouch::setVolume(uint8_t left, uint8_t right) {
  uint16_t vol = 0x00;
  vol |= VOL_MAX - Clamp(left, VOL_MIN, VOL_MAX);
  vol = vol << 8;
  vol |= VOL_MAX - Clamp(right, VOL_MIN, VOL_MAX);
  player.setVolume(vol);
}

void CocoTouch::playMIDINote(uint8_t note, uint8_t velocity = 127,
                             uint8_t channel = 0) {
  player.midiNoteOn(channel, note, velocity);
}

void CocoTouch::stopMIDINote(uint8_t note, uint8_t velocity = 127,
                             uint8_t channel = 0) {
  player.midiNoteOff(channel, note, velocity);
}

uint16_t CocoTouch::getTouchPinValue(uint8_t pinNum) {
  return cap.filteredData(pinNum);
}

uint16_t CocoTouch::getTouched(uint8_t pinNum) {
  return (cap.touched() & _BV(pinNum)) > 0 ? 1 : 0;
}

uint16_t CocoTouch::getTouchPinBaseValue(uint8_t pinNum) {
  return cap.baselineData(pinNum);
}

void CocoTouch::setMIDIProgram(uint8_t program, uint8_t channel = 0) {
  player.midiWriteData((MIDI_SET_PROGRAM | channel), program, 0);
}

void CocoTouch::sendBleData(uint8_t data[], size_t size) {
  bleSerial.write(data, size);
}

size_t CocoTouch::writeBLE(const char *str) {
  return bleSerial.write(str, strlen(str));
}

size_t CocoTouch::sendFlashMessage(const char *s) {
  size_t len = strlen_P(s);
  for (int k = 0; k < len; k++) {
    char myChar = pgm_read_byte(s + k);
    bleSerial.write(myChar);
  }
  bleSerial.write((uint8_t)EOD);
  return len + 3;
}

size_t CocoTouch::sendFlashMessage(const __FlashStringHelper *ifsh) {
  PGM_P p = reinterpret_cast<PGM_P>(ifsh);
  size_t n = 0;
  while (1) {
    unsigned char c = pgm_read_byte(p++);
    if (c == 0) break;
    if (bleSerial.write(c))
      n++;
    else
      break;
  }
  bleSerial.write((uint8_t)EOD);
  return n + 3;
}

size_t CocoTouch::sendMessage(const String &s) {
  size_t length = bleSerial.write(s.c_str(), s.length());
  bleSerial.write((uint8_t)EOD);
  return length;
}

void CocoTouch::setMIDIBank(uint8_t bank) {
  player.midiWriteData(MIDI_SET_BANK, 0x00, bank);
}
