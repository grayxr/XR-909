#ifndef XROpenHat_h
#define XROpenHat_h

#include <Audio.h>
#include <Arduino.h>

#include "XRKitInstrument.h"

class OpenHat : public KitInstrument {    
  public:
    const unsigned int *sample;

    byte decay;
    
    AudioPlayMemory &mem;
    AudioMixer4 &mix;
    AudioEffectEnvelope &env;
    
    OpenHat(
      const unsigned int *sample,
      AudioPlayMemory &mem,
      AudioMixer4 &mix,
      AudioEffectEnvelope &env
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

#endif /* XROpenHat_h */
