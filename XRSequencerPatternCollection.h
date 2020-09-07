#ifndef XRSequencerPatternCollection_h
#define XRSequencerPatternCollection_h

#include "XRConfig.h"
#include "XRSequencerPattern.h"

#include <Arduino.h>
#include <vector>

class SequencerPatternCollection {
  private:
    std::vector<SequencerPattern *> collection;
    
  public:
    SequencerPatternCollection() : collection() {
        collection.reserve(PATTERN_LIMIT);

        int idx = 0;
        for (unsigned int i = 0; i < PATTERN_LIMIT; ++i) {
            collection.push_back (new SequencerPattern (idx));
            ++idx;
        }
    }
    
    std::vector<SequencerPattern *> getAll();
};

#endif /* XRSequencerPatternCollection_h */
