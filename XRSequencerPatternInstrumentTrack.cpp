#include "XRSequencerPatternInstrumentTrack.h"
#include "XRSequencerPatternInstrumentTrackStep.h"

#include <Arduino.h>
#include <vector>

void SequencerPatternInstrumentTrack::writePatternInstrumentTrack() {
  // enable pattern ins. track write mode, now all changes get persisted ...
}

SequencerPatternInstrumentTrackStep *SequencerPatternInstrumentTrack::getInstrumentTrackStepAtIndex(int idx) {
    return this->patternInstrumentTrackSteps.at(idx);
}

void SequencerPatternInstrumentTrack::doInstrumentTrackStepAtIndex(KitInstrument *instrument, int idx) {
    this->patternInstrumentTrackSteps.at(idx)->doTriggerForInstrument(instrument);
}
