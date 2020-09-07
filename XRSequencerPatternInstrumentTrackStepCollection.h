#ifndef XRSequencerPatternInstrumentTrackCollectionStep_h
#define XRSequencerPatternInstrumentTrackCollectionStep_h

#include "XRConfig.h"

#include "XRSequencerPatternInstrumentTrackStep.h"

#include <Arduino.h>
#include <vector>

class SequencerPatternInstrumentTrackStepCollection {
  private:
    std::vector<SequencerPatternInstrumentTrackStep *> collection;
    
  public:
    SequencerPatternInstrumentTrackStepCollection(int patternIdx, int patternTrackIdx) : collection() {
        collection.reserve(PATTERN_INSTRUMENT_TRACK_STEP_LIMIT);

        int idx = 0;
        for (unsigned int i = 0; i < PATTERN_INSTRUMENT_TRACK_STEP_LIMIT; ++i) {
            collection.push_back(new SequencerPatternInstrumentTrackStep (patternIdx, patternTrackIdx, idx));
            ++idx;
        }
    }

    std::vector<SequencerPatternInstrumentTrackStep *> getAll();
};

#endif /* XRSequencerPatternInstrumentTrackCollectionStep_h */
