#ifndef XRSequencerPatternInstrumetTrack_h
#define XRSequencerPatternInstrumetTrack_h

#include "XRConfig.h"

#include "XRKitInstrument.h"

#include "XRSequencerPatternInstrumentTrackStep.h"
#include "XRSequencerPatternInstrumentTrackStepCollection.h"

#include <Arduino.h>
#include <vector>

/**
 * A "pattern instrument track" is a one-dimensional sequence, 
 * associated with a "kit instrument", composed of "pattern instrument track steps"
 */

class SequencerPatternInstrumentTrack {
   public:
    int patternIndex = 0; // keep track of the pattern that this track is associated with
    int patternTrackIndex = 0;  // keep track of the index of this instrument track
    
    SequencerPatternInstrumentTrackStepCollection *patternInstrumentTrackStepCollection;
    
    std::vector<SequencerPatternInstrumentTrackStep *> patternInstrumentTrackSteps;

    // Constructor
    SequencerPatternInstrumentTrack(int patternIdx, int patternTrackIndex) {
        this->patternIndex = patternIdx;
        this->patternTrackIndex = patternTrackIndex;
      
        // not DI :(
        this->patternInstrumentTrackStepCollection = new SequencerPatternInstrumentTrackStepCollection(
          this->patternIndex,
          this->patternTrackIndex
        );
        
        this->patternInstrumentTrackSteps = this->patternInstrumentTrackStepCollection->getAll();
    }

    SequencerPatternInstrumentTrackStep *getInstrumentTrackStepAtIndex(int idx);

    void doInstrumentTrackStepAtIndex(KitInstrument *instrument, int index);
    
    void writePatternInstrumentTrack();
};

#endif /* XRSequencerPatternInstrumetTrack_h */
