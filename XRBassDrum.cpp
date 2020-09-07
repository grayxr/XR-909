#include "XRUtils.h"
#include "XRBassDrum.h"
#include <Arduino.h>

static float CLIPPED_TRI_SHAPE[9] = {
  -0.41275,
  -0.39625,
  -0.25,
  -0.1,
  0,
  0.1,
  0.25,
  0.39625,
  0.41275
};

BassDrum::BassDrum(
  AudioSynthWaveformDc &dc,
  AudioEffectEnvelope &pitchEnv,
  AudioSynthWaveformModulated &osc,
  AudioEffectWaveshaper &oscShaper,
  AudioFilterStateVariable &oscFilter,
  AudioEffectEnvelope &oscEnv,
  AudioSynthWaveformDc &pulseDc,
  AudioEffectEnvelope &pulseEnv,
  AudioSynthWaveformModulated &pulseOsc,
  AudioSynthNoiseWhite &noise,
  AudioFilterStateVariable &noiseFilter,
  AudioMixer4 &attackMixer,
  AudioEffectEnvelope &attackEnv,
  AudioMixer4 &mainMixer
) : dc {dc},
    pitchEnv {pitchEnv},
    osc {osc},
    oscShaper {oscShaper},
    oscFilter {oscFilter},
    oscEnv {oscEnv},
    pulseDc {pulseDc},
    pulseEnv {pulseEnv},
    pulseOsc {pulseOsc},
    noise {noise},
    noiseFilter {noiseFilter},
    attackMixer {attackMixer},
    attackEnv {attackEnv},
    mainMixer {mainMixer}
{
  this->tune = 0;
  this->attack = 0;
  this->decay = 0;

  this->releaseWindow = 350;
}

void BassDrum::init() {

  /**
   * Drum section
   */
   
  // Tune
  this->tune = 42; // TODO: get init tune from pot 

  // Drum Pitch Env
  this->dc.amplitude(0.25);
  this->pitchEnv.attack(2);
  this->pitchEnv.decay(40);
  this->pitchEnv.sustain(0);
  this->pitchEnv.release(80);

  // Drum Osc
  this->osc.begin(WAVEFORM_TRIANGLE);
  this->osc.amplitude(0);
  this->osc.frequency(getMainFreqFromTune());
  this->oscShaper.shape(CLIPPED_TRI_SHAPE, 9);
  this->oscFilter.frequency(4000);

  // Drum Osc Env (TODO: modulate with Accent?)
  this->oscEnv.decay(40);
  this->oscEnv.release(350);
  
  /**
   * Attack section
   */
  
  // Pulse DC
  this->pulseDc.amplitude(0.8);

  // Pulse Envelope
  this->pulseEnv.attack(1);
  this->pulseEnv.decay(2);
  this->pulseEnv.release(3);

  // Pulse Osc
  this->pulseOsc.begin(WAVEFORM_SINE);
  this->pulseOsc.amplitude(0.2);
  this->pulseOsc.frequency(10);

  // Subtle noise
  this->noise.amplitude(0.1);
  this->noiseFilter.frequency(900);

  // Attack envelope
  this->attackEnv.attack(3);
  this->attackEnv.decay(25);
  this->attackEnv.release(50);
  
  /**
   * Mix section
   */

  // Mix together voice and attack
  this->mainMixer.gain(0, 1);
  this->mainMixer.gain(1, 1);

  KitInstrument::init();
}

void BassDrum::trigger(int accent) {
    KitInstrument::trigger(0); // ignore accent on parent method
  
    float amp = map(accent, 1, 2, 5, 10) * 0.1;
    this->osc.amplitude(amp);
    
    this->pulseEnv.noteOn();
    this->attackEnv.noteOn();
    this->pitchEnv.noteOn();
    this->oscEnv.noteOn();
}

void BassDrum::release() {
    this->pitchEnv.noteOff();
    this->oscEnv.noteOff();
    this->pulseEnv.noteOff();
    this->attackEnv.noteOff();
}

void BassDrum::setParameterValues(int paramPot1Val, int paramPot2Val, int paramPot3Val) {
   this->setTune(paramPot1Val);
   this->setAttack(paramPot2Val);
   this->setDecay(paramPot3Val);
}

void BassDrum::setTune(byte value) {
  this->tune = map(value, 0, 127, 0, 60);

  this->osc.frequency(getMainFreqFromTune());

  int valCapped = max(50, value);
  float dcAmt = map(valCapped, 0, 127, 0, 33) * 0.01;  
  this->dc.amplitude(dcAmt);
}

void BassDrum::setAttack(byte value) {
  this->attack = map(value, 0, 127, 50, 100);

  this->mainMixer.gain(1, this->attack * 0.01);
}

void BassDrum::setDecay(byte value) {
  this->decay = value;

  int dDecay = min(108, max(96, this->decay));
  int mapRel = map(this->decay, 0, 127, 0, 400);

  this->oscEnv.decay(dDecay);
  this->oscEnv.release(mapRel);
}

float BassDrum::getMainFreqFromTune() {
  float tuneFreq = map(this->tune, 0, 60, 45, 75);
  
  return tuneFreq;
}

/**
 * Ratchet methods
 */

void BassDrum::resetDecay() {
  if (this->quickDecayModified == false) {
    return;
  }
  
  this->setDecay(this->decay);

  this->quickDecayModified = false;
}

void BassDrum::setQuickDecay(int ms) {
  this->quickDecayModified = true;
  
  this->oscEnv.decay(ms+20);
  this->oscEnv.release(ms+300);
}
