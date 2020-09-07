#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioSynthWaveformDc     bdDc;          //xy=708.3333396911621,563.3333406448364
AudioSynthNoiseWhite     bdNoise;        //xy=707.0237464904785,898.9047737121582
AudioSynthWaveformModulated bdOsc;         //xy=715.3333396911621,644.3333406448364
AudioSynthWaveformDc     bdPulseDc;            //xy=718.0951232910156,758.3334445953369
AudioEffectEnvelope      bdPulseEnv; //xy=722.380916595459,796.9048280715942
AudioSynthWaveformModulated bdPulseOsc; //xy=723.8095512390137,835.4761781692505
AudioFilterStateVariable bdNoiseFilter;  //xy=726.0237464904785,944.9047737121582
AudioPlayMemory          chMem;          //xy=724.642879486084,1454.5714302062988
AudioEffectEnvelope      bdPitchEnv;    //xy=728.3333396911621,602.3333406448364
AudioSynthNoiseWhite     sdNoise;        //xy=727.3571281433105,1302.9999732971191
AudioPlayMemory          ohMem; //xy=727.1427955627441,1565.7142333984375
AudioSynthWaveformDc     sdPitchDc1;     //xy=733.3571281433105,1041.9999732971191
AudioEffectWaveshaper    bdOscWaveshape;     //xy=734.9999694824219,686.6666736602783
AudioSynthWaveformModulated sdOsc1;         //xy=734.3571281433105,1217.9999732971191
AudioSynthWaveform       sdPitchOsc1;    //xy=736.3571281433105,1117.9999732971191
AudioSynthWaveform       sdPitchOsc2;    //xy=736.3571281433105,1157.9999732971191
AudioSynthWaveformModulated sdOsc2;         //xy=736.3571281433105,1255.9999732971191
AudioEffectEnvelope      sdPitchEnv1;    //xy=740.3571281433105,1077.9999732971191
AudioEffectEnvelope      sdNoiseEnv1;    //xy=884.3571281433105,1300.9999732971191
AudioSynthNoiseWhite     sdSizzleNoise;  //xy=892.3571281433105,1356.9999732971191
AudioEffectEnvelope      sdOsc1env1;     //xy=894.3571281433105,1217.9999732971191
AudioEffectEnvelope      sdOsc2env1;     //xy=896.3571281433105,1258.9999732971191
AudioMixer4              sdPitchMixer1;  //xy=926.3571281433105,1140.9999732971191
AudioFilterStateVariable bdOscFilter;        //xy=941.6666641235352,694.9999465942383
AudioMixer4              bdAttackMixer;  //xy=942.7857437133789,854.7618751525879
AudioEffectEnvelope      sdOsc1env2;     //xy=1052.3571281433105,1219.9999732971191
AudioEffectEnvelope      sdOsc2env2;     //xy=1055.3571281433105,1258.9999732971191
AudioFilterBiquad        sdNoiseFilter;        //xy=1056.6666870117188,1298.3333854675293
AudioEffectEnvelope      sdNoiseEnv2;    //xy=1060.3571281433105,1357.9999732971191
AudioEffectEnvelope      bdAttackEnv;    //xy=1145.9759521484375,849.6190528869629
AudioEffectEnvelope      bdOscEnv;     //xy=1151.8571853637695,701.0000076293945
AudioMixer4              sdOscMixer;     //xy=1242.3571281433105,1234.9999732971191
AudioMixer4              sdNoiseMixer;   //xy=1247.3571281433105,1305.9999732971191
AudioMixer4              bdMainMixer;    //xy=1359.9285125732422,719.9999694824219
AudioMixer4              chMix;          //xy=1382.6428604125977,1473.5714645385742
AudioMixer4              ohMix; //xy=1385.1427764892578,1584.714267730713
AudioFilterStateVariable sdOscFilter;    //xy=1398.3571281433105,1240.9999732971191
AudioEffectEnvelope      chEnv;      //xy=1519.1428031921387,1478.9048042297363
AudioEffectFade          chFade;          //xy=1601.666690826416,1378.3333492279053
AudioMixer4              sdMainMixer;    //xy=1609.3571281433105,1309.9999732971191
AudioEffectEnvelope      ohEnv; //xy=1613.3094291687012,1583.3809223175049
AudioMixer4              mainMixer;      //xy=1801.3571281433105,1314.9999732971191
AudioOutputI2S           outputDevice;   //xy=1968.3571281433105,1315.9999732971191
AudioConnection          patchCord1(bdDc, bdPitchEnv);
AudioConnection          patchCord2(bdNoise, 0, bdNoiseFilter, 0);
AudioConnection          patchCord3(bdOsc, bdOscWaveshape);
AudioConnection          patchCord4(bdPulseDc, bdPulseEnv);
AudioConnection          patchCord5(bdPulseEnv, 0, bdPulseOsc, 0);
AudioConnection          patchCord6(bdPulseOsc, 0, bdAttackMixer, 0);
AudioConnection          patchCord7(bdNoiseFilter, 0, bdAttackMixer, 1);
AudioConnection          patchCord8(chMem, 0, chMix, 0);
AudioConnection          patchCord9(bdPitchEnv, 0, bdOsc, 0);
AudioConnection          patchCord10(sdNoise, sdNoiseEnv1);
AudioConnection          patchCord11(ohMem, 0, ohMix, 0);
AudioConnection          patchCord12(sdPitchDc1, sdPitchEnv1);
AudioConnection          patchCord13(bdOscWaveshape, 0, bdOscFilter, 0);
AudioConnection          patchCord14(sdOsc1, sdOsc1env1);
AudioConnection          patchCord15(sdPitchOsc1, 0, sdPitchMixer1, 1);
AudioConnection          patchCord16(sdPitchOsc2, 0, sdPitchMixer1, 2);
AudioConnection          patchCord17(sdOsc2, sdOsc2env1);
AudioConnection          patchCord18(sdPitchEnv1, 0, sdPitchMixer1, 0);
AudioConnection          patchCord19(sdNoiseEnv1, sdNoiseFilter);
AudioConnection          patchCord20(sdSizzleNoise, sdNoiseEnv2);
AudioConnection          patchCord21(sdOsc1env1, sdOsc1env2);
AudioConnection          patchCord22(sdOsc2env1, sdOsc2env2);
AudioConnection          patchCord23(sdPitchMixer1, 0, sdOsc1, 0);
AudioConnection          patchCord24(sdPitchMixer1, 0, sdOsc2, 0);
AudioConnection          patchCord25(bdOscFilter, 0, bdOscEnv, 0);
AudioConnection          patchCord26(bdAttackMixer, bdAttackEnv);
AudioConnection          patchCord27(sdOsc1env2, 0, sdOscMixer, 0);
AudioConnection          patchCord28(sdOsc2env2, 0, sdOscMixer, 1);
AudioConnection          patchCord29(sdNoiseFilter, 0, sdNoiseMixer, 0);
AudioConnection          patchCord30(sdNoiseEnv2, 0, sdNoiseMixer, 2);
AudioConnection          patchCord31(bdAttackEnv, 0, bdMainMixer, 1);
AudioConnection          patchCord32(bdOscEnv, 0, bdMainMixer, 0);
AudioConnection          patchCord33(sdOscMixer, 0, sdOscFilter, 0);
AudioConnection          patchCord34(sdNoiseMixer, 0, sdMainMixer, 1);
AudioConnection          patchCord35(bdMainMixer, 0, mainMixer, 0);
AudioConnection          patchCord36(chMix, chEnv);
AudioConnection          patchCord37(ohMix, ohEnv);
AudioConnection          patchCord38(sdOscFilter, 0, sdMainMixer, 0);
AudioConnection          patchCord39(chEnv, chFade);
AudioConnection          patchCord40(chFade, 0, mainMixer, 2);
AudioConnection          patchCord41(sdMainMixer, 0, mainMixer, 1);
AudioConnection          patchCord42(ohEnv, 0, mainMixer, 3);
AudioConnection          patchCord43(mainMixer, 0, outputDevice, 0);
AudioConnection          patchCord44(mainMixer, 0, outputDevice, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=1949.5,1263
// GUItool: end automatically generated code
