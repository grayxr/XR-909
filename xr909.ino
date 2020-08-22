#include "XRAudioComponents.h"
#include "XRKit.h"

Kit *kit;

void setup() {
  if (Serial) {
    Serial.begin(115200);
  }
  
  AudioMemory(20);  
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.9);

  // init final main mixer
  mainMixer.gain(0, 1.0);
  mainMixer.gain(1, 1.0);
  // TODO refactor, add more channels

  buildKit();
}

void loop() {
  //kit->testBass();
  //kit->testSnare();

  kit->testKit();
}

void buildKit() {
  kit = new Kit(
    buildBass(),
    buildSnare()
  );
  kit->init();
}

Bass* buildBass() {
  return new Bass(
    bassPitchDc1,
    bassPitchEnv1,
    bassOsc1,
    bassOsc1waveShape,
    bassOsc1env1,
    bassOsc1amp,
    bassNoise,
    bassNoiseEnv1,
    bassMainMixer
  );
}

Snare* buildSnare() {
  return new Snare(
    snarePitchDc1,
    snarePitchOsc1,
    snarePitchOsc2,
    snarePitchEnv1,
    snarePitchMixer1,
    snareOsc1,
    snareOsc2,
    snareOsc1env1,
    snareOsc1env2,
    snareOsc1amp,
    snareOsc2env1,
    snareOsc2env2,
    snareOsc2amp,
    snareNoise,
    snareNoiseEnv1,
    snareMainMixer
  );
}
