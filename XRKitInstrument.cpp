#include "XRKitInstrument.h"

void KitInstrument::doRelease() {
  if (this->triggerTime > 0 && millis() >= (this->triggerTime + this->releaseWindow)) { // make release window dynamic
    this->release();

    this->triggerTime = 0;
  }
}

unsigned int KitInstrument::getPosition() {
   return this->position;
}
