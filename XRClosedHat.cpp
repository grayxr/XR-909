#include "XRUtils.h"
#include "XRClosedHat.h"
#include <Arduino.h>

#include "XRKitInstrument.h"

ClosedHat::ClosedHat(
    const unsigned int *sample,
    AudioPlayMemory &mem,
    AudioMixer4 &mix,
    AudioEffectEnvelope &env,
    AudioEffectFade &fade
) : sample {sample},
    mem {mem},
    mix {mix},
    env {env},
    fade {fade}
{
  this->position = 2;
  
  this->decay = 0;
  this->releaseWindow = 70;
}

void ClosedHat::init() {
  // scan pots for init values eventually
  this->env.attack(0);
  this->env.decay(100);
  this->env.sustain(0);
  this->env.release(70);

  KitInstrument::init();
}

void ClosedHat::trigger(int accent) {
    KitInstrument::trigger(0);
  
    float nAccent = (float) accent; // don't need accent
    nAccent = 0.8;
    
    this->mem.play(this->sample);
    this->mix.gain(0, nAccent);
    this->env.noteOn();
}

void ClosedHat::release() {
    this->env.noteOff();
}

void ClosedHat::setParameterValues(int paramPot1Val, int paramPot2Val, int paramPot3Val) {
    this->setDecay(paramPot1Val);
}

void ClosedHat::setDecay(byte value) {
  this->decay = value;
  
  float chDecay = map(this->decay, 0, 127, 40, 100);
  this->env.decay(chDecay);
  this->env.release(70); // hard-coded for now

  //float chFade = map(this->decay, 0, 127, 20, 127);
  this->fade.fadeOut(30);
  this->fade.fadeIn(1);
}

/**
 * Ratchet methods
 */

void ClosedHat::resetDecay() {
  if (this->quickDecayModified == false) {
    return;
  }
  
  this->setDecay(this->decay);

  this->quickDecayModified = false;
}

void ClosedHat::setQuickDecay(int ms) {
  this->quickDecayModified = true;

  ms += 20;
  int relMs = ms+100;
  
  this->env.decay(ms);
  this->env.release(relMs);
}
