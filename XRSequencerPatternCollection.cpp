#include "XRSequencerPatternCollection.h"
#include "XRSequencerPattern.h"

#include <Arduino.h>
#include <vector>

std::vector<SequencerPattern *> SequencerPatternCollection::getAll() {  
  return this->collection;
}
