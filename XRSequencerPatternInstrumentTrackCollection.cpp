#include "XRSequencerPatternInstrumentTrackCollection.h"
#include "XRSequencerPatternInstrumentTrack.h"

#include "XRConfig.h"

#include <Arduino.h>
#include <vector>

std::vector<SequencerPatternInstrumentTrack *> SequencerPatternInstrumentTrackCollection::getAll() {
  return this->collection;
}
