#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioSynthNoiseWhite     commonNoise;         //xy=80.75,1164.2500143051147
AudioSynthNoiseWhite     snareNoise; //xy=275,1162.5
AudioSynthNoiseWhite     bassNoise; //xy=278.75000762939453,761.2500114440918
AudioSynthWaveformModulated bassOsc1;   //xy=434,716
AudioSynthWaveformDc     snarePitchDc1;            //xy=435.25001525878906,897.5000305175781
AudioSynthWaveformModulated snareOsc1;   //xy=436,1073.5
AudioSynthWaveform       snarePitchOsc1;      //xy=438.25000762939453,973.5000152587891
AudioSynthWaveformModulated snareOsc2;   //xy=438,1111.5
AudioSynthWaveformDc     bassPitchDc1; //xy=440.25001525878906,634.2500247955322
AudioSynthWaveform       snarePitchOsc2; //xy=438.75000762939453,1013.7500152587891
AudioEffectEnvelope      snarePitchEnv1;      //xy=442.25000762939453,933.5000152587891
AudioEffectEnvelope      bassPitchEnv1; //xy=447.25000762939453,670.2500095367432
AudioEffectEnvelope      snareNoiseEnv1;      //xy=447.5,1162.5
AudioEffectEnvelope      bassNoiseEnv1; //xy=450,761
AudioEffectEnvelope      snareOsc1env1;      //xy=596,1073.5000095367432
AudioEffectEnvelope      snareOsc2env1;      //xy=598,1114.5
AudioEffectWaveshaper    bassOsc1waveShape;     //xy=622.5000076293945,715.0000114440918
AudioMixer4              snarePitchMixer1;         //xy=628.0000076293945,996.5000152587891
AudioEffectEnvelope      snareOsc1env2;      //xy=777,1073.5
AudioEffectEnvelope      snareOsc2env2;      //xy=780,1112.5
AudioEffectEnvelope      bassOsc1env1;      //xy=817.7500152587891,716.0000104904175
AudioAmplifier           snareOsc1amp;           //xy=953,1073.5
AudioAmplifier           snareOsc2amp;           //xy=955,1112.5
AudioAmplifier           bassOsc1amp;           //xy=994.5000152587891,716.0000114440918
AudioMixer4              bassMainMixer; //xy=1173.500015258789,761.0000114440918
AudioMixer4              snareMainMixer;         //xy=1203.5,1156.5
AudioMixer4              mainMixer;         //xy=1516,1163
AudioOutputI2S           outputDevice;           //xy=1683.0000228881836,1164.2500133514404
AudioConnection          patchCord1(snareNoise, snareNoiseEnv1);
AudioConnection          patchCord2(bassNoise, bassNoiseEnv1);
AudioConnection          patchCord3(bassOsc1, bassOsc1env1);
AudioConnection          patchCord4(snarePitchDc1, snarePitchEnv1);
AudioConnection          patchCord5(snareOsc1, snareOsc1env1);
AudioConnection          patchCord6(snarePitchOsc1, 0, snarePitchMixer1, 1);
AudioConnection          patchCord7(snareOsc2, snareOsc2env1);
AudioConnection          patchCord8(bassPitchDc1, bassPitchEnv1);
AudioConnection          patchCord9(snarePitchOsc2, 0, snarePitchMixer1, 2);
AudioConnection          patchCord10(snarePitchEnv1, 0, snarePitchMixer1, 0);
AudioConnection          patchCord11(bassPitchEnv1, 0, bassOsc1, 0);
AudioConnection          patchCord12(snareNoiseEnv1, 0, snareMainMixer, 2);
AudioConnection          patchCord13(bassNoiseEnv1, 0, bassMainMixer, 1);
AudioConnection          patchCord14(snareOsc1env1, snareOsc1env2);
AudioConnection          patchCord15(snareOsc2env1, snareOsc2env2);
AudioConnection          patchCord16(bassOsc1waveShape, bassOsc1env1);
AudioConnection          patchCord17(snarePitchMixer1, 0, snareOsc1, 0);
AudioConnection          patchCord18(snarePitchMixer1, 0, snareOsc2, 0);
AudioConnection          patchCord19(snareOsc1env2, snareOsc1amp);
AudioConnection          patchCord20(snareOsc2env2, snareOsc2amp);
AudioConnection          patchCord21(bassOsc1env1, bassOsc1amp);
AudioConnection          patchCord22(snareOsc1amp, 0, snareMainMixer, 0);
AudioConnection          patchCord23(snareOsc2amp, 0, snareMainMixer, 1);
AudioConnection          patchCord24(bassOsc1amp, 0, bassMainMixer, 0);
AudioConnection          patchCord25(bassMainMixer, 0, mainMixer, 0);
AudioConnection          patchCord26(snareMainMixer, 0, mainMixer, 1);
AudioConnection          patchCord27(mainMixer, 0, outputDevice, 0);
AudioConnection          patchCord28(mainMixer, 0, outputDevice, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=581.0000152587891,250.00001335144043
// GUItool: end automatically generated code
