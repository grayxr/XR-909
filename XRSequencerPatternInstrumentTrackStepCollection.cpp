#include "XRSequencerPatternInstrumentTrackStepCollection.h"
#include "XRSequencerPatternInstrumentTrackStep.h"

#include "XRConfig.h"

#include <Arduino.h>
#include <vector>

std::vector<SequencerPatternInstrumentTrackStep *> SequencerPatternInstrumentTrackStepCollection::getAll() {
  return this->collection;
}
