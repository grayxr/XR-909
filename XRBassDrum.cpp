#include "XRBassDrum.h"
#include "XRUtils.h"
#include <Arduino.h>

Bass::Bass(
  AudioSynthWaveformDc &pitchDc1,
  AudioEffectEnvelope &pitchEnv1,
  AudioSynthWaveformModulated &osc1,
  AudioEffectWaveshaper &osc1waveShape,
  AudioEffectEnvelope &osc1env1,
  AudioAmplifier &osc1amp,
  AudioSynthNoiseWhite &noise,
  AudioEffectEnvelope &noiseEnv1,
  AudioMixer4 &mainMixer
) : pitchDc1 {pitchDc1},
    pitchEnv1 {pitchEnv1},
    osc1 {osc1},
    osc1waveShape {osc1waveShape},
    osc1env1 {osc1env1},
    osc1amp {osc1amp},
    noise {noise},
    noiseEnv1 {noiseEnv1},
    mainMixer {mainMixer}
{
  this->note = 0;
  this->velocity = 0;
}

void Bass::init() {
  this->pitchDc1.amplitude(0.25);
  this->pitchEnv1.attack(0);
  this->pitchEnv1.decay(75);
  this->pitchEnv1.sustain(0);
  this->pitchEnv1.release(200);
  
  this->osc1.begin(WAVEFORM_TRIANGLE);
  this->osc1.amplitude(1);
  this->osc1.frequency(0);

//  float BASS_WAVESHAPE[17] = {
//    -0.788,
//    -0.779,
//    -0.649,
//    -0.588,
//    -0.496,
//    -0.320,
//    -0.228,
//    -0.122,
//    0,
//    0.122,
//    0.228,
//    0.320,
//    0.496,
//    0.588,
//    0.649,
//    0.779,
//    0.788
//  };
  //this->osc1waveShape.shape(BASS_WAVESHAPE, 17);
  
  this->osc1env1.attack(0);
  this->osc1env1.decay(110);
  this->osc1env1.sustain(0);
  this->osc1env1.release(300);
  this->osc1amp.gain(0.7);
  
  this->noise.amplitude(0.01);
  this->noiseEnv1.attack(5);
  this->noiseEnv1.decay(150);
  this->noiseEnv1.sustain(0);
  this->noiseEnv1.release(180);

  this->mainMixer.gain(0, 1.0);
  this->mainMixer.gain(1, 0.005);

  this->initialized = true;
}

void Bass::play() {
  this->osc1.frequency(noteFreqs[this->note]);
  
  float velo = (this->velocity * DIV127);
  this->osc1.amplitude(velo);
  this->noise.amplitude(0.1);

  this->pitchEnv1.noteOn();
  this->osc1env1.noteOn();
  this->noiseEnv1.noteOn();
}

void Bass::stop() {
  this->pitchEnv1.noteOff();
  this->osc1env1.noteOff();
  this->noiseEnv1.noteOff();
}
