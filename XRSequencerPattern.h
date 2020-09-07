#ifndef XRSequencerPattern_h
#define XRSequencerPattern_h

#include "XRConfig.h"

#include "XRSequencerPatternInstrumentTrack.h"
#include "XRSequencerPatternInstrumentTrackCollection.h"

#include "XRKit.h"

#include <Arduino.h>
#include <vector>

class SequencerPattern {
   public:
    int pKeyIndex = 0; // track which key location this pattern is assigned to
    
    int patternLength;
    int currentPatternStepIndex;
    int currentPatternSectionIndex;
    
    SequencerPatternInstrumentTrackCollection *patternInstrumentTracksCollection;
    std::vector<SequencerPatternInstrumentTrack *> patternInstrumentTracks;

    // Constructor
    SequencerPattern(int pKeyIdx) {
        this->pKeyIndex = pKeyIdx;

        // not DI :(
        this->patternInstrumentTracksCollection = new SequencerPatternInstrumentTrackCollection(this->pKeyIndex);
        this->patternInstrumentTracks = this->patternInstrumentTracksCollection->getAll();
    }

    void doStep(Kit *kit);
    void onStart();

    void writePattern();
};

#endif /* XRSequencerPattern_h */
