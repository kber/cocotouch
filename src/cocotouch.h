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

#ifndef COCOTOUCH_H
#define COCOTOUCH_H

#include <WString.h>
#include <stdint.h>
#include <string.h>

#define MIDI_BANK_GM1 0x79
#define MIDI_BANK_GM2 0x78
#define MIDI_PROG_ACOUSTIC_GRAND_PIANO 0x01
#define MIDI_SET_BANK 0xb0
#define MIDI_SET_PROGRAM 0xc0
#define TRUE (1 == 1)
#define FALSE !TRUE

#define PIN_DAT_IN 7
#define PIN_LOAD 3 // CS
#define PIN_CLK 6

#define LED_SIZE 8

#define AUDIO_MODE_MIDI 0x00
#define AUDIO_MODE_FILE 0x01

// ADDR default: 0x5A
// ADDR tied to 3V: 0x5B
// ADDR tied to SDA: 0x5C
// ADDR tied to SCL: 0x5D
#define I2CADDR 0x5A

#define BLE_SPEED 9600

class CocoTouch {
public:
  bool begin();
  bool beginAudio();
  bool beginBLE(long speed = BLE_SPEED);
  bool beginMIDI();
  bool beginTouchSensor(uint8_t i2cAddress = I2CADDR);
  void setAudioMode(uint8_t mode = AUDIO_MODE_MIDI);
  uint16_t getTouchPinValue(uint8_t pinNum);
  uint16_t getTouchPinBaseValue(uint8_t pinNum);
  void playAudioFile(const char *name);
  void playAudioFiles();
  void playMIDINote(uint8_t note, uint8_t velocity = 127, uint8_t channel = 0);
  void stopMIDINote(uint8_t note, uint8_t velocity = 127, uint8_t channel = 0);
  void setMIDIProgram(uint8_t program, uint8_t channel = 0);
  void setVolume(uint8_t vol);
  void setVolume(uint8_t left, uint8_t right);
  void sendBleData(uint8_t data[], size_t size);
  size_t writeBLE(const char *str);

  size_t sendMessage(const String &s);
  size_t sendFlashMessage(const char *s);
  size_t sendFlashMessage(const __FlashStringHelper *ifsh);
  uint16_t getTouched(uint8_t pinNum);

private:
  void setMIDIBank(uint8_t bank);
};

#endif