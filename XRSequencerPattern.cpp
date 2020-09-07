#include "XRConfig.h"

#include "XRSequencerPattern.h"
#include "XRSequencerPatternInstrumentTrack.h"

#include "XRKit.h"

#include <Arduino.h>

void SequencerPattern::writePattern() {
  // enable pattern write mode, now all changes get persisted ...
}

void SequencerPattern::doStep(Kit *kit) {
  // do steps for every pattern instrument track for the current pattern step
  for (auto &instrumentTrack : this->patternInstrumentTracks) {
    instrumentTrack->doInstrumentTrackStepAtIndex(
      kit->getInstrumentByTrack(instrumentTrack->patternTrackIndex), 
      this->currentPatternStepIndex
    );
  }

  if (this->currentPatternStepIndex < 4) {
      this->currentPatternSectionIndex = 0;
  } else if (this->currentPatternStepIndex < 8) {
      this->currentPatternSectionIndex = 1;
  } else if (this->currentPatternStepIndex < 12) {
      this->currentPatternSectionIndex = 2;
  } else if (this->currentPatternStepIndex < 16) {
      this->currentPatternSectionIndex = 3;
  }


 
  // increment and/or reset pattern step index
  if (++this->currentPatternStepIndex == PATTERN_STEP_LIMIT) {
      this->currentPatternStepIndex = 0;
  }
}

// Make sure always starting on first step
void SequencerPattern::onStart() {
  this->currentPatternStepIndex = 0;
}
