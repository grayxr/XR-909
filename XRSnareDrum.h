#include <Audio.h>
#include <Arduino.h>

class Snare {    
  public:
    byte note;
    byte velocity;

    bool initialized = false;
    
    AudioSynthWaveformDc &pitchDc1;
    AudioSynthWaveform &pitchOsc1;
    AudioSynthWaveform &pitchOsc2;
    AudioEffectEnvelope &pitchEnv1;
    AudioMixer4 &pitchMixer1;
    AudioSynthWaveformModulated &osc1;
    AudioSynthWaveformModulated &osc2;
    AudioEffectEnvelope &osc1env1;
    AudioEffectEnvelope &osc1env2;
    AudioAmplifier &osc1amp;
    AudioEffectEnvelope &osc2env1;
    AudioEffectEnvelope &osc2env2;
    AudioAmplifier &osc2amp;
    AudioSynthNoiseWhite &noise;
    AudioEffectEnvelope &noiseEnv1;
    AudioMixer4 &mainMixer;
    
    Snare(
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
    );
    
    void init();
    void play();
    void stop();

    // USB MIDI handlers
    
//    virtual void noteOnHandler(byte channel, byte note, byte velocity);
//    virtual void noteOffHandler(byte channel, byte note, byte velocity);
//    virtual void ccHandler(byte channel, byte control, byte value);
};
