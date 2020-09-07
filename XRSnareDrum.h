#include <Audio.h>
#include <Arduino.h>

#include "XRKitInstrument.h"

extern const int NOISE_MIXER_CHANNEL_LPF;
extern const int NOISE_MIXER_CHANNEL_HPF;
extern const int MIXER_CHANNEL_DRUM;
extern const int MIXER_CHANNEL_NOISE;

class SnareDrum : public KitInstrument {    
  public:
    byte tune;
    byte tone;
    byte snappy;
    
    AudioSynthWaveformDc &pitchDc1;
    AudioSynthWaveform &pitchOsc1;
    AudioSynthWaveform &pitchOsc2;
    AudioEffectEnvelope &pitchEnv1;
    AudioMixer4 &pitchMixer1;
    AudioSynthWaveformModulated &osc1;
    AudioSynthWaveformModulated &osc2;
    AudioEffectEnvelope &osc1env1;
    AudioEffectEnvelope &osc1env2;
    AudioEffectEnvelope &osc2env1;
    AudioEffectEnvelope &osc2env2;
    AudioMixer4 &oscMixer;
    AudioFilterStateVariable &oscFilter;
    AudioSynthNoiseWhite &noise;
    AudioSynthNoiseWhite &sizzleNoise;
    AudioEffectEnvelope &noiseEnv1;
    AudioEffectEnvelope &noiseEnv2;
    AudioFilterBiquad &noiseFilter;
    AudioMixer4 &noiseMixer;
    AudioMixer4 &mainMixer;
    
    SnareDrum(
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
    );
    
    void setTune(byte value);
    void setTone(byte value);
    void setSnappy(byte value);
    
    float getMainFreqFromTune();
    float getSubFreqFromTune();

    int noiseEnvDecay;
    int noiseEnvRelease;

    // kit instrument methods
    
    void init();
    void trigger(int accent);
    void release();

    void setParameterValues(int paramPot1Val, int paramPot2Val, int paramPot3Val);
    void setQuickDecay(int ms);
    void resetDecay();
};
