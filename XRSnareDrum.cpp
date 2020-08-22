#include "XRSnareDrum.h"
#include "XRUtils.h"
#include <Arduino.h>

Snare::Snare(
  AudioSynthWaveformDc &pitchDc1,
  AudioSynthWaveform &pitchOsc1,
  AudioSynthWaveform &pitchOsc2,
  AudioEffectEnvelope &pitchEnv1,
  AudioMixer4 &pitchMixer1,
  AudioSynthWaveformModulated &osc1,
  AudioSynthWaveformModulated &osc2,
  AudioEffectEnvelope &osc1env1,
  AudioEffectEnvelope &osc1env2,
  AudioAmplifier &osc1amp,
  AudioEffectEnvelope &osc2env1,
  AudioEffectEnvelope &osc2env2,
  AudioAmplifier &osc2amp,
  AudioSynthNoiseWhite &noise,
  AudioEffectEnvelope &noiseEnv1,
  AudioMixer4 &mainMixer
) : pitchDc1 {pitchDc1},
    pitchOsc1 {pitchOsc1},
    pitchOsc2 {pitchOsc2},
    pitchEnv1 {pitchEnv1},
    pitchMixer1 {pitchMixer1},
    osc1 {osc1},
    osc2 {osc2},
    osc1env1 {osc1env1},
    osc1env2 {osc1env2},
    osc1amp {osc1amp},
    osc2env1 {osc2env1},
    osc2env2 {osc2env2},
    osc2amp {osc2amp},
    noise {noise},
    noiseEnv1 {noiseEnv1},
    mainMixer {mainMixer}
{
  this->note = 0;
  this->velocity = 0;
}

void Snare::init() {
  this->pitchDc1.amplitude(0.125);
  this->pitchOsc1.begin(WAVEFORM_SINE);
  this->pitchOsc1.amplitude(0.0055);
  this->pitchOsc1.frequency(25);
  this->pitchOsc1.pulseWidth(0.5);
  this->pitchOsc2.begin(WAVEFORM_SINE);
  this->pitchOsc2.amplitude(0.01);
  this->pitchOsc2.frequency(0.005);
  this->pitchOsc2.pulseWidth(0.5);
  this->pitchEnv1.attack(5);
  this->pitchEnv1.decay(15);
  this->pitchEnv1.sustain(0);
  this->pitchEnv1.release(20);
  
  this->osc1.begin(WAVEFORM_TRIANGLE);
  this->osc1.amplitude(0);
  this->osc1.frequency(0);
  this->osc1env1.attack(0);
  this->osc1env1.decay(125);
  this->osc1env1.sustain(0);
  this->osc1env1.release(500);

  this->osc1env2.delay(0);
  this->osc1env2.attack(0);
  this->osc1env2.decay(101);
  this->osc1env2.sustain(0);
  this->osc1env2.release(300);
  
  this->osc1amp.gain(0.5);

  this->osc2.begin(WAVEFORM_TRIANGLE);
  this->osc2.amplitude(0);
  this->osc2.frequency(0);
  this->osc2env1.attack(0);
  this->osc2env1.decay(125);
  this->osc2env1.sustain(0);
  this->osc2env1.release(500);
  
  this->osc2env2.delay(0);
  this->osc2env2.attack(0);
  this->osc2env2.decay(101);
  this->osc2env2.sustain(0);
  this->osc2env2.release(300);
  
  this->osc2amp.gain(0.5); 
  
  this->noise.amplitude(0);
  this->noiseEnv1.attack(5);
  this->noiseEnv1.decay(120);
  this->noiseEnv1.sustain(0);
  this->noiseEnv1.release(100);

  this->mainMixer.gain(0, 1);
  this->mainMixer.gain(1, 1);
  this->mainMixer.gain(2, 0.05);

  this->initialized = true;
}

void Snare::play() {
  this->pitchOsc1.phase(0);
  
  this->osc1.frequency(noteFreqs[this->note]);
  this->osc2.frequency(noteFreqs[this->note + 7]);
  
  float velo = (this->velocity * DIV127);
  this->osc1.amplitude(velo);
  this->osc2.amplitude(velo - 0.1);
  this->noise.amplitude(1);

  this->pitchEnv1.noteOn();
  this->osc1env1.noteOn();
  this->osc1env2.noteOn();
  this->osc2env1.noteOn();
  this->osc2env2.noteOn();
  this->noiseEnv1.noteOn();
}

void Snare::stop() {
  this->pitchEnv1.noteOff();
  this->osc1env1.noteOff();
  this->osc1env2.noteOff();
  this->osc2env1.noteOff();
  this->osc2env2.noteOff();
  this->noiseEnv1.noteOff();
}

// TODO: Implement USB MIDI handlers
//
//void Snare::noteOnHandler(byte channel, byte note, byte velocity) {
//  if (note > 23 && note < 108) {
//    this->note = note;
//    this->velocity = velocity;
//    play();
//  }
//}
//
//void Snare::noteOffHandler(byte channel, byte note, byte velocity) {
//  if (note > 23 && note < 108) {
//    stop();
//  }
//}
//
//void Snare::ccHandler(byte channel, byte control, byte value) {
//  switch (control) {
//    case 100:
//      this->mainMixer.gain(0, (value * DIV127));
//      break;
//    case 101:
//      this->mainMixer.gain(1, (value * DIV127));
//      break;
//    case 102:
//      //mixer1.gain(2, (value * DIV127));
//      this->mainMixer.gain(2, 0.5);
//      break;
//      
//    case 103:
//      this->osc1env1.attack(3000 * (value * DIV127));
//      this->osc2env1.attack(3000 * (value * DIV127));
//      this->noiseEnv1.attack(3000 * (value * DIV127));
//      break;
//    case 104:
//      this->osc1env1.decay(3000 * (value * DIV127));
//      this->osc2env1.decay(3000 * (value * DIV127));
//      this->noiseEnv1.decay(3000 * (value * DIV127));
//      break;
//    case 105:
//      this->osc1env1.sustain(value * DIV127);
//      this->osc2env1.sustain(value * DIV127);
//      break;
//    case 106:
//      this->osc1env1.release(3000 * (value * DIV127));
//      this->osc2env1.release(3000 * (value * DIV127));
//      break;
//  }
//}
