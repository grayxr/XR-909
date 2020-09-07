#ifndef XRSequencerPatternInstrumentTrackCollection_h
#define XRSequencerPatternInstrumentTrackCollection_h

#include "XRConfig.h"

#include "XRSequencerPatternInstrumentTrack.h"
#include "XRSequencerPatternInstrumentTrackStep.h"

#include <Arduino.h>
#include <vector>

class SequencerPatternInstrumentTrackCollection {
  private:
    std::vector<SequencerPatternInstrumentTrack *> collection;
    
  public:
    SequencerPatternInstrumentTrackCollection(int patternIdx) : collection() {
        collection.reserve(PATTERN_INSTRUMENT_TRACK_LIMIT);

        int idx = 0;
        for (unsigned int i = 0; i < PATTERN_INSTRUMENT_TRACK_LIMIT; ++i) {
            collection.push_back(new SequencerPatternInstrumentTrack (patternIdx, idx));
            ++idx;
        }
    }

    std::vector<SequencerPatternInstrumentTrack *> getAll();
};

#endif /* XRSequencerPatternInstrumentTrackCollection_h */
