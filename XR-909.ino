#include "XRConfig.h"
#include "XRAudioComponents.h"

#include "XRKit.h"
#include "XRSequencer.h"
#include "XRSequencerPatternCollection.h"

Kit *kit;
Sequencer *seq;

ResponsiveAnalogRead paramPot1(A16, true);
ResponsiveAnalogRead paramPot2(A17, true);
ResponsiveAnalogRead paramPot3(A14, true);
ResponsiveAnalogRead levelPot(A15, true);
ResponsiveAnalogRead ratchetPot(A11, true);
ResponsiveAnalogRead mainVolPot(A13, true);

/**
 * TODOs:
 * 
 * 1. Implement binary step on/off states for samples-based instruments
 * 1. Fix pattern play mode / pattern group select, enable it and:
 *    - remove LED display logic which slowly advances LEDs for every 4th step, we already will see the pattern section progression via the 7 seg
 *    - instead, just turn on the LED that corresponds to the currently active pattern
 *    - get pattern selection working
 *    - MAYBE: try handling count-in / queue-up feature, where current pattern finishes before jumping to next one
 * 1. Get 7 seg display working and make an updateState function for keeping the display up to date
 *    - Always display current pattern section in left most digit, "0xxx", "1xxx", etc.
 *    - when global knob updates occur (volume , tempo, accent), then show these values in the remaining 3 digits
 * 2. Implement passive instrument controls, or only update instrument parameters when:
 *    - controls have been modified within a certain error margin (5-10 values from current set value)
 *    - a different instrument has been selected and only when a knob has been turned, 
 *      not immediately when switching to the new instrument
 * 3. Enable pattern section paging/cursor feature
 *    - Figure out key combo to enter and exit paging/cursor mode
 *    - Shift+start/stop goes forward or back in pattern section view
 *    - The 7 seg display leftmost digit should lock to the currently cursored section and blink intermittently
 *    - While in pattern section cursor mode, 
 *      enable fill feature for filling all 16 steps with current step properties (accent, on/off state, etc)
 */

void setup() {
  if (Serial) {
    Serial.begin(BAUD_RATE);
    Serial.println("Serial enabled!");
  }
  
  // wait a bit
  delay(1000);

  setupSound();
  
  setupKit();
  setupSeq();
}

void loop() {
  seq->updateState();
  kit->updateState();
  
  #ifdef DEBUG
    debugAudioUsage();
  #endif
}

void setupSound() {
  AudioMemory(20);  
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.9);
}

void setupKit() {
  kit = new Kit(
    new BassDrum(
      bdDc,
      bdPitchEnv,
      bdOsc,
      bdOscWaveshape,
      bdOscFilter,
      bdOscEnv,
      bdPulseDc,
      bdPulseEnv,
      bdPulseOsc,
      bdNoise,
      bdNoiseFilter,
      bdAttackMixer,
      bdAttackEnv,
      bdMainMixer
    ),
    new SnareDrum(
      sdPitchDc1,
      sdPitchOsc1,
      sdPitchOsc2,
      sdPitchEnv1,
      sdPitchMixer1,
      sdOsc1,
      sdOsc2,
      sdOsc1env1,
      sdOsc1env2,
      sdOsc2env1,
      sdOsc2env2,
      sdOscMixer,
      sdOscFilter,
      sdNoise,
      sdSizzleNoise,
      sdNoiseEnv1,
      sdNoiseEnv2,
      sdNoiseFilter,
      sdNoiseMixer,
      sdMainMixer
    ),
    new ClosedHat(
      chSample,
      chMem,
      chMix,
      chEnv,
      chFade
    ),
    new OpenHat(
      ohSample,
      ohMem,
      ohMix,
      ohEnv
    ),
    mainMixer,
    paramPot1,
    paramPot2,
    paramPot3,
    ratchetPot,
    levelPot
  );
}

void setupSeq() {
  seq = new Sequencer(
    kit,
    new SequencerPatternCollection()
  );
}

void debugAudioUsage() {
  Serial.print("CPU: ");
  Serial.print("all=");
  Serial.print(AudioProcessorUsage());
  Serial.print(",");
  Serial.print(AudioProcessorUsageMax());
  Serial.print("    ");
  Serial.print("Memory: ");
  Serial.print(AudioMemoryUsage());
  Serial.print(",");
  Serial.print(AudioMemoryUsageMax());
  Serial.println();
}
