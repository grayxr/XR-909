#ifndef XRKit_h
#define XRKit_h

#include "XRConfig.h"
#include "XRKitInstrument.h"
#include "XRBassDrum.h"
#include "XRSnareDrum.h"
#include "XRClosedHat.h"
#include "XRClosedHatSample.h"
#include "XROpenHat.h"
#include "XROpenHatSample.h"

#include "Bounce2.h"
#include <Arduino.h>

class Kit {
  public:
    BassDrum *bassDrum;
    SnareDrum *snareDrum;
    ClosedHat *closedHat;
    OpenHat *openHat;
    AudioMixer4 &mix;
    ResponsiveAnalogRead paramPot1;
    ResponsiveAnalogRead paramPot2;
    ResponsiveAnalogRead paramPot3;
    ResponsiveAnalogRead ratchetPot;
    ResponsiveAnalogRead levelPot;

    Button triggerBtn;

    unsigned long runtime;
    unsigned long triggerBtnReadTime;
    unsigned long latestInputCheckTime;
    unsigned long latestRatchetTriggerTime;

    bool isRatcheting = false;

    KitInstrument *currentInstrument;

    Kit(
      BassDrum *bassDrum,
      SnareDrum *snareDrum,
      ClosedHat *closedHat,
      OpenHat *openHat,
      AudioMixer4 &mix,
      ResponsiveAnalogRead paramPot1,
      ResponsiveAnalogRead paramPot2,
      ResponsiveAnalogRead paramPot3,
      ResponsiveAnalogRead ratchetPot,
      ResponsiveAnalogRead levelPot
    );

    void updateState(); // loop function
    void updateInstrumentControls();

    KitInstrument *getInstrumentByTrack(int trackIdx);

    void setCurrentInstrumentByTrack(int trackIdx);
    void setMixerLevel(int channel, byte value);
    
    void handleInstrumentControls();
    void handleInstrumentReleases();
    void handleTriggerControls();
    void handleRatchet(byte value);
    void stopRatchet();
    
    void init();
};

#endif /* XRKit_h */
