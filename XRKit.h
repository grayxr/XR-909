#include "XRBassDrum.h"
#include "XRSnareDrum.h"

class Kit {
  public:
    Bass *bass;
    Snare *snare;

    Kit(
      Bass *bass,
      Snare *snare
    );
    
    void init();
    void testKit();
    void testBass();
    void testSnare();
    void testBoth();
};
