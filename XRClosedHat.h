#ifndef XRClosedHat_h
#define XRClosedHat_h

#include <Audio.h>
#include <Arduino.h>

#include "XRKitInstrument.h"

class ClosedHat : public KitInstrument {    
  public:
    const unsigned int *sample;

    byte decay;
    
    AudioPlayMemory &mem;
    AudioMixer4 &mix;
    AudioEffectEnvelope &env;
    AudioEffectFade &fade;
    
    ClosedHat(
      const unsigned int *sample,
      AudioPlayMemory &mem,
      AudioMixer4 &mix,
      AudioEffectEnvelope &env,
      AudioEffectFade &fade
    );
    
    void setDecay(byte value);

    // kit instrument methods
    
    void init();
    void trigger(int accent);
    void release();

    void setParameterValues(int paramPot1Val, int paramPot2Val, int paramPot3Val);
    void setQuickDecay(int ms);
    void resetDecay();
};

#endif /* XRClosedHat_h */
