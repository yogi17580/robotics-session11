#include "AudioTools.h"
#include "BluetoothA2DPSink.h"

// Audio goes out through I2S to the amplifier
I2SStream i2s;
BluetoothA2DPSink speaker(i2s);

// Pins for the knob and buttons
#define VOLUME_KNOB 34
#define PREV_BUTTON 14
#define PLAY_BUTTON 12
#define NEXT_BUTTON 13

int oldVolume = -1;

void setup() {
  // Buttons: pressed = LOW
  pinMode(PREV_BUTTON, INPUT_PULLUP);
  pinMode(PLAY_BUTTON, INPUT_PULLUP);
  pinMode(NEXT_BUTTON, INPUT_PULLUP);

  // Pins that go to the amp
  auto cfg = i2s.defaultConfig();
  cfg.pin_bck  = 26;   // BCLK
  cfg.pin_ws   = 25;   // LRC
  cfg.pin_data = 22;   // DIN
  i2s.begin(cfg);

  // Start Bluetooth. Pair with "ESP32-Speaker" on your phone.
  speaker.start("ESP32-Speaker");
}

void loop() {
  // Volume knob
  int newVolume = analogRead(VOLUME_KNOB) * 127 / 4095;
  if (abs(newVolume - oldVolume) >= 2) {   // only change if the knob really moved
    speaker.set_volume(newVolume);
    oldVolume = newVolume;
  }

  // Play / Pause button
  if (digitalRead(PLAY_BUTTON) == LOW) {
    if (speaker.get_audio_state() == ESP_A2D_AUDIO_STATE_STARTED) speaker.pause();
    else speaker.play();
    delay(300);   // wait so one press counts once
  }

  // Next button
  if (digitalRead(NEXT_BUTTON) == LOW) {
    speaker.next();
    delay(300);
  }

  // Previous button
  if (digitalRead(PREV_BUTTON) == LOW) {
    speaker.previous();
    delay(300);
  }

  delay(20);
}