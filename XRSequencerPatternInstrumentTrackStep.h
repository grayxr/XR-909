#ifndef XRSequencerPatternInstrumetTrackStep_h
#define XRSequencerPatternInstrumetTrackStep_h

#include "XRKitInstrument.h"

#include <Arduino.h>
#include <vector>

// A "pattern instrument track step" is an object composed of an on/off assignment (with an accent option, when it is used)

enum class InstrumentTrackStepState {
  OFF = 0, // you only have to specify the first value, the compiler does the rest, but w/e
  ON = 1,
  ACCENTED = 2
};

class SequencerPatternInstrumentTrackStep {
   public:
    int patternIndex = 0; // associated pattern
    int patternTrackIndex = 0; // associated pattern ins. track
    int patternTrackStepIndex = 0; // keep track of the index of this step
    
    int state = (int) InstrumentTrackStepState::OFF; // default state

    SequencerPatternInstrumentTrackStep(int patternIdx, int patternTrackIdx, int patternTrackStepIdx) {
        this->patternIndex = patternIdx;
        this->patternTrackIndex = patternTrackIdx;
        this->patternTrackStepIndex = patternTrackStepIdx;
    }
    
    int toggleTriggerState();
    int getState();
    
    void setState(int state);
    void doTriggerForInstrument(KitInstrument *instrument);
};

#endif /* XRSequencerPatternInstrumetTrackStep_h */
