#ifndef XRKitInstrument_h
#define XRKitInstrument_h

#include<Arduino.h>

class KitInstrument {    
  public:
    bool initialized = false;
    bool quickDecayModified = false;

    unsigned long int triggerTime;
    unsigned int releaseWindow;
    
    unsigned int position;

    KitInstrument() {}

    void doRelease();
    
    unsigned int getPosition();

    virtual void init() { this->releaseWindow = 100; this->initialized = true; };
    virtual void trigger(int accent) { this->triggerTime = millis(); };
    virtual void release();
    virtual void setParameterValues(int paramPot1Val, int paramPot2Val, int paramPot3Val);

    // Needed for ratcheting / quicker noteOn & noteOff windows
    virtual void setQuickDecay(int ms);
    virtual void resetDecay();
};

#endif /* XRKitInstrument_h */
