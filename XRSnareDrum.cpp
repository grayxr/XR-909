#include "XRSnareDrum.h"
#include "XRUtils.h"
#include <Arduino.h>

const int NOISE_MIXER_CHANNEL_LPF = 0;
const int NOISE_MIXER_CHANNEL_HPF = 1;
const int MIXER_CHANNEL_DRUM = 0;
const int MIXER_CHANNEL_NOISE = 1;

// these should remain consts, to emulate the real snare sound
const float oscEnvDecay = 105;
const float oscEnvRelease = 400;
const float noiseEnvAttack = 4;

// TODO: set from init pot values
const float noiseEnvDecay = 90;
const float noiseEnvRelease = 200;

SnareDrum::SnareDrum(
  AudioSynthWaveformDc &pitchDc1,
  AudioSynthWaveform &pitchOsc1,
  AudioSynthWaveform &pitchOsc2,
  AudioEffectEnvelope &pitchEnv1,
  AudioMixer4 &pitchMixer1,
  AudioSynthWaveformModulated &osc1,
  AudioSynthWaveformModulated &osc2,
  AudioEffectEnvelope &osc1env1,
  AudioEffectEnvelope &osc1env2,
  AudioEffectEnvelope &osc2env1,
  AudioEffectEnvelope &osc2env2,
  AudioMixer4 &oscMixer,
  AudioFilterStateVariable &oscFilter,
  AudioSynthNoiseWhite &noise,
  AudioSynthNoiseWhite &sizzleNoise,
  AudioEffectEnvelope &noiseEnv1,
  AudioEffectEnvelope &noiseEnv2,
  AudioFilterBiquad &noiseFilter,
  AudioMixer4 &noiseMixer,
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
    osc2env1 {osc2env1},
    osc2env2 {osc2env2},
    oscMixer {oscMixer},
    oscFilter {oscFilter},
    noise {noise},
    sizzleNoise {sizzleNoise},
    noiseEnv1 {noiseEnv1},
    noiseEnv2 {noiseEnv2},
    noiseFilter {noiseFilter},
    noiseMixer {noiseMixer},
    mainMixer {mainMixer}
{
  this->position = 1;
  
  this->tune = 0;
  this->tone = 0;
  this->snappy = 0;

  this->noiseEnvDecay = noiseEnvDecay;
  this->noiseEnvRelease = noiseEnvRelease;

  this->releaseWindow = 100;

  //this->slop = 0; // modified by holding TRIGGER btn + turning TUNE pot -- adds slop to the second OSC freq
}

void SnareDrum::init() {

  /**
   * Voice section
   */
  
  this->tune = 52; // TODO: get init tune from pot 
  
  // Pitch env
  this->pitchDc1.amplitude(0.1);
  this->pitchEnv1.attack(2);
  this->pitchEnv1.decay(23);
  this->pitchEnv1.sustain(0);
  this->pitchEnv1.release(100); // no effect...

  // Put some warble and drift in the sound
  this->pitchOsc1.begin(WAVEFORM_SINE);
  this->pitchOsc1.amplitude(0.0055);
  this->pitchOsc1.frequency(25);
  this->pitchOsc1.pulseWidth(0.5);
  this->pitchOsc2.begin(WAVEFORM_SINE);
  this->pitchOsc2.amplitude(0.01);
  this->pitchOsc2.frequency(0.005);
  this->pitchOsc2.pulseWidth(0.5);

  // Low-tune Osc
  this->osc1.begin(WAVEFORM_TRIANGLE);
  this->osc1.amplitude(0);
  this->osc1.frequency(getMainFreqFromTune());
  this->osc1env1.attack(0);
  this->osc1env1.decay(oscEnvDecay);
  this->osc1env1.sustain(0);
  this->osc1env1.release(oscEnvRelease);

  this->osc1env2.attack(0);
  this->osc1env2.decay(oscEnvDecay);
  this->osc1env2.sustain(0);
  this->osc1env2.release(180);

  // Hi-tune Osc
  this->osc2.begin(WAVEFORM_TRIANGLE);
  this->osc2.amplitude(0);
  this->osc2.frequency(getSubFreqFromTune());
  this->osc2env1.attack(0);
  this->osc2env1.decay(oscEnvDecay);
  this->osc2env1.sustain(0);
  this->osc2env1.release(oscEnvRelease);
  
  this->osc2env2.delay(0);
  this->osc2env2.attack(0);
  this->osc2env2.decay(oscEnvDecay);
  this->osc2env2.sustain(0);
  this->osc2env2.release(180);

  // Make the low-tuned osc more prominent
  this->oscMixer.gain(0,1.0);
  this->oscMixer.gain(1,0.8);

  // Make the overall osc sound warmer
  this->oscFilter.frequency(7000);

  /**
   * Snappy section
   */
  
  this->noise.amplitude(0.5);
  //this->sizzleNoise.amplitude(0);
  
  this->noiseEnv1.attack(noiseEnvAttack);
  this->noiseEnv1.decay(noiseEnvDecay);
  this->noiseEnv1.sustain(0);
  this->noiseEnv1.release(noiseEnvRelease);

  // TODO: init "snappy" mixer by scanning pot values
  //this->noiseMixer.gain(NOISE_MIXER_CHANNEL_LPF, 0.0); // low-pass / less snappy
  //this->noiseMixer.gain(NOISE_MIXER_CHANNEL_HPF, 0.0); // high-pass / more snappy
  //this->noiseMixer.gain(2, 0.001);

  /**
   * Mix section
   */

  this->mainMixer.gain(MIXER_CHANNEL_DRUM, 0.5);
  this->mainMixer.gain(MIXER_CHANNEL_NOISE, 0.5);

  // TODO: read from pot
  setSnappy(25);
  setTone(62);

  KitInstrument::init();
}

void SnareDrum::trigger(int accent) {
    KitInstrument::trigger(0);
    
    float amp = map(accent, 1, 2, 5, 10) * 0.1;
    
    this->osc1.amplitude(amp);
    this->osc2.amplitude(amp);
    this->noise.amplitude(amp / 2);
  
    this->pitchEnv1.noteOn();
    this->osc1env1.noteOn();
    this->osc1env2.noteOn();
    this->osc2env1.noteOn();
    this->osc2env2.noteOn();
    this->noiseEnv1.noteOn();
    this->noiseEnv2.noteOn();
}

void SnareDrum::release() {
    this->pitchEnv1.noteOff();
    this->osc1env1.noteOff();
    this->osc1env2.noteOff();
    this->osc2env1.noteOff();
    this->osc2env2.noteOff();
    this->noiseEnv1.noteOff();
    this->noiseEnv2.noteOff();
}

void SnareDrum::setParameterValues(int paramPot1Val, int paramPot2Val, int paramPot3Val) {
   this->setTune(paramPot1Val);
   this->setTone(paramPot2Val);
   this->setSnappy(paramPot3Val);
}

void SnareDrum::setTune(byte value) {
  this->tune = map(value, 0, 127, 0, 60); // map 0-127 to 0-60?

  this->osc1.frequency(getMainFreqFromTune());
  this->osc2.frequency(getSubFreqFromTune());
}

void SnareDrum::setTone(byte value) {
  this->tone = value;

  int intValue = value;

  float toneDecNew = map(intValue, 0, 127, 89, 105);
  float toneDecAmount = max(toneDecNew, 0);  
  this->noiseEnv1.decay(toneDecAmount);
  this->noiseEnvDecay = toneDecAmount;

  float toneRelNew = map(intValue, 0, 127, 200, 350);
  float toneRelAmount = max(toneRelNew, 0);  
  this->noiseEnv1.release(toneRelAmount);
  this->noiseEnvRelease = toneRelAmount;
}

void SnareDrum::setSnappy(byte value) {
  this->snappy = value;

  int intValue = (int) value;
  
  this->noiseFilter.setNotch(0, 100);  // band-pass
  this->noiseMixer.gain(0, min(value, 100) * 0.01);  // band-pass
 
  float noiseNew = map(intValue, 0, 127, 0, 100) * 0.01;
  float noiseGainAmount = max(noiseNew, 0);  
  this->mainMixer.gain(MIXER_CHANNEL_NOISE, noiseGainAmount);
}

float SnareDrum::getMainFreqFromTune() {
  float tuneFreq = map(this->tune, 0, 60, 100, 250);
  
  return tuneFreq;
}

float SnareDrum::getSubFreqFromTune() {
  return map(this->tune, 0, 60, 100, 250) * 1.499; // fifth (refactor to include slop factor, set by holding trigger and turning tune)
}

void SnareDrum::setQuickDecay(int ms) {
  this->quickDecayModified = true;

  ms += 20;
  int relMs = ms + 180;
  
  this->osc1env1.decay(ms);
  this->osc1env1.release(relMs);
  
  this->osc1env2.decay(ms);
  this->osc1env2.release(relMs);
  
  this->osc2env1.decay(ms);
  this->osc2env1.release(relMs);
  
  this->osc2env2.decay(ms);
  this->osc2env2.release(relMs);
  
  this->noiseEnv1.attack(0);
  this->noiseEnv1.decay(ms);
  this->noiseEnv1.sustain(0);
  this->noiseEnv1.release(min(relMs, 200));
}

void SnareDrum::resetDecay() {
  if (this->quickDecayModified == false) {
    return;
  }
  
  this->osc1env1.decay(oscEnvDecay);
  this->osc1env1.release(oscEnvRelease);
  
  this->osc1env2.decay(oscEnvDecay);
  this->osc1env2.release(oscEnvRelease);
  
  this->osc2env1.decay(oscEnvDecay);
  this->osc2env1.release(oscEnvRelease);
  
  this->osc2env2.decay(oscEnvDecay);
  this->osc2env2.release(oscEnvRelease);
  
  this->noiseEnv1.attack(noiseEnvAttack);
  this->noiseEnv1.decay(this->noiseEnvDecay);
  this->noiseEnv1.release(this->noiseEnvRelease);

  this->quickDecayModified = false;
}
