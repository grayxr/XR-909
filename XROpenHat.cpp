#include "XROpenHat.h"
#include "XRUtils.h"
#include <Arduino.h>

OpenHat::OpenHat(
    const unsigned int *sample,
    AudioPlayMemory &mem,
    AudioMixer4 &mix,
    AudioEffectEnvelope &env
) : sample {sample},
    mem {mem},
    mix {mix},
    env {env}
{
  this->position = 3;
  
  this->decay = 0;
  this->releaseWindow = 160;
}

void OpenHat::init() {
  this->env.attack(0);
  this->env.decay(175);
  this->env.sustain(0);
  this->env.release(160);

  KitInstrument::init();
}

void OpenHat::trigger(int accent) {
    KitInstrument::trigger(0);
  
    float nAccent = accent; // don't need accent
    nAccent = 0.8;
    
    this->mem.play(this->sample);
    this->mix.gain(0, nAccent);
    this->env.noteOn();
}

void OpenHat::release() {
    this->env.noteOff();
}

void OpenHat::setParameterValues(int paramPot1Val, int paramPot2Val, int paramPot3Val) {
    this->setDecay(paramPot1Val);
}

void OpenHat::setDecay(byte value) {
  this->decay = value;
  
  float ohDecay = map(this->decay, 0, 127, 100, 200);
  this->env.decay(ohDecay);

  float ohRel = map(this->decay, 0, 127, 40, 200);
  this->env.release(ohRel);
}

/**
 * Ratchet methods
 */

void OpenHat::resetDecay() {
  if (this->quickDecayModified == false) {
    return;
  }
  
  this->setDecay(this->decay);

  this->quickDecayModified = false;
}

void OpenHat::setQuickDecay(int ms) {
  this->quickDecayModified = true;
  
  this->env.decay(ms+20);
  this->env.release(ms+100);
}
