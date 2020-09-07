#ifndef XRBassDrum_h
#define XRBassDrum_h

#include <Audio.h>
#include <Arduino.h>

#include "XRKitInstrument.h"

class BassDrum : public KitInstrument {    
  public:
    unsigned int position = 0;
  
    byte tune;
    byte attack;
    byte decay;
    
    AudioSynthWaveformDc &dc;
    AudioEffectEnvelope &pitchEnv;
    AudioSynthWaveformModulated &osc;
    AudioEffectWaveshaper &oscShaper;
    AudioFilterStateVariable &oscFilter;
    AudioEffectEnvelope &oscEnv;
    AudioSynthWaveformDc &pulseDc;
    AudioEffectEnvelope &pulseEnv;
    AudioSynthWaveformModulated &pulseOsc;
    AudioSynthNoiseWhite &noise;
    AudioFilterStateVariable &noiseFilter;
    AudioMixer4 &attackMixer;
    AudioEffectEnvelope &attackEnv;
    AudioMixer4 &mainMixer;
    
    BassDrum(
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
    );

    void setTune(byte value);
    void setAttack(byte value);
    void setDecay(byte value);
    
    float getMainFreqFromTune();

    // kit instrument methods
    
    void init();
    void trigger(int accent);
    void release();

    void setParameterValues(int paramPot1Val, int paramPot2Val, int paramPot3Val);
    void setQuickDecay(int ms);
    void resetDecay();
};

#endif /* XRBassDrum_h */
