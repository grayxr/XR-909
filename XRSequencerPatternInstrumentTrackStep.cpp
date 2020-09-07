#include "XRSequencerPatternInstrumentTrackStep.h"

#include "XRKitInstrument.h"

#include <Arduino.h>

void SequencerPatternInstrumentTrackStep::setState(int state) {
    this->state = state;
}

int SequencerPatternInstrumentTrackStep::getState() {
    return (int) this->state;
}

int SequencerPatternInstrumentTrackStep::toggleTriggerState() {
    switch (this->state) {
        case (int) InstrumentTrackStepState::OFF :
          this->state = (int) InstrumentTrackStepState::ON;
          break;
        case (int) InstrumentTrackStepState::ON :
          this->state = (int) InstrumentTrackStepState::ACCENTED;
          break;
        case (int) InstrumentTrackStepState::ACCENTED :
          this->state = (int) InstrumentTrackStepState::OFF;
          break;
    }

    return this->state;
}

void SequencerPatternInstrumentTrackStep::doTriggerForInstrument(KitInstrument *instrument) {
    if (state == (int) InstrumentTrackStepState::OFF) {
        //Serial.println("instrument track step state = OFF");
        return;
    }
  
    instrument->trigger(state);
}
