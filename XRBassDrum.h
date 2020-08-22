#include <Audio.h>
#include <Arduino.h>

class Bass {    
  public:
    byte note;
    byte velocity;

    bool initialized = false;
    
    AudioSynthWaveformDc &pitchDc1;
    AudioEffectEnvelope &pitchEnv1;
    AudioSynthWaveformModulated &osc1;
    AudioEffectWaveshaper &osc1waveShape;
    AudioEffectEnvelope &osc1env1;
    AudioAmplifier &osc1amp;
    AudioSynthNoiseWhite &noise;
    AudioEffectEnvelope &noiseEnv1;
    AudioMixer4 &mainMixer;
    
    Bass(
      AudioSynthWaveformDc &pitchDc1,
      AudioEffectEnvelope &pitchEnv1,
      AudioSynthWaveformModulated &osc1,
      AudioEffectWaveshaper &osc1waveShape,
      AudioEffectEnvelope &osc1env1,
      AudioAmplifier &osc1amp,
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
