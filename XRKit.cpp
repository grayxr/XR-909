#include "XRKit.h"
#include "XRKitInstrument.h"
#include <Arduino.h>

Kit::Kit(
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
) : mix {mix} {
  this->bassDrum = bassDrum;
  this->snareDrum = snareDrum;
  this->closedHat = closedHat;
  this->openHat = openHat;
  
  this->levelPot = levelPot;
  this->paramPot1 = paramPot1;
  this->paramPot2 = paramPot2;
  this->paramPot3 = paramPot3;
  this->ratchetPot = ratchetPot;
  
  // TODO: scan pots for init values
  this->bassDrum->init();
  this->snareDrum->init();
  this->closedHat->init();
  this->openHat->init();

  this->mix.gain(0, 0.82); // BD
  this->mix.gain(1, 1);    // SD
  this->mix.gain(2, 0.1);  // CH
  this->mix.gain(3, 0.1);  // CH

  this->triggerBtn = Button();
  this->triggerBtn.attach(28, INPUT_PULLDOWN);
  this->triggerBtn.interval(25);
  this->triggerBtn.setPressedState(LOW);

  this->currentInstrument = this->getInstrumentByTrack(0);
}

void Kit::setCurrentInstrumentByTrack(int trackIdx) {
  Serial.print("setting current instrument track by index: ");
  Serial.println(trackIdx);
  this->currentInstrument = this->getInstrumentByTrack(trackIdx);
}

KitInstrument *Kit::getInstrumentByTrack(int trackIdx) {
  switch (trackIdx) {
    case 0:
      return this->bassDrum;
      break;
    case 1:
      return this->snareDrum;
      break;
    case 2:
      return this->closedHat;
      break;
    case 3:
      return this->openHat;
      break;
  }

  return this->bassDrum;
}

// Gets called every loop
void Kit::updateState(){
    this->runtime = millis();

    this->updateInstrumentControls();
    
    this->handleInstrumentControls();
    this->handleInstrumentReleases();
}

void Kit::updateInstrumentControls() {
    this->paramPot1.update();
    this->paramPot2.update();
    this->paramPot3.update();
    this->ratchetPot.update();
    this->levelPot.update();
    this->triggerBtn.update();
}

void Kit::handleInstrumentControls(){
  if (this->latestInputCheckTime == 0 || 
    ((this->runtime - this->latestInputCheckTime) >= 50 && (this->runtime - this->latestInputCheckTime) <= 100)
  ) {
      this->latestInputCheckTime = this->runtime;
      
      this->currentInstrument->setParameterValues(
          map(this->paramPot1.getValue(), 10, 1000, 100, 0),
          map(this->paramPot2.getValue(), 10, 1000, 100, 0),
          map(this->paramPot3.getValue(), 10, 1000, 100, 0)
      );
      
//      Serial.print("param pot 1: ");
//      Serial.print(map(this->paramPot1.getValue(), 10, 1000, 100, 0));
//      Serial.print(" param pot 2: ");
//      Serial.print(map(this->paramPot2.getValue(), 10, 1000, 100, 0));
//      Serial.print(" param pot 3: ");
//      Serial.print(map(this->paramPot3.getValue(), 10, 1000, 100, 0));
//      Serial.print(" level pot: ");
//      Serial.print(map(this->levelPot.getValue(), 10, 1000, 100, 0));
//
//      Serial.print(" current instrument position: ");
//      Serial.println(this->currentInstrument->getPosition());

      this->setMixerLevel(
        this->currentInstrument->getPosition(), 
        map(this->levelPot.getValue(), 10, 1000, 100, 0)
      );
  }

  this->handleTriggerControls();
}

void Kit::handleTriggerControls() {
  if (this->triggerBtnReadTime == 0 && (this->triggerBtn.read())) {
    this->triggerBtnReadTime = this->runtime;
    
  } else if (this->triggerBtn.read() == false) {
    this->triggerBtnReadTime = 0;
    this->isRatcheting = false;
  }
  
  if (this->triggerBtn.read() && (this->isRatcheting || (this->runtime - this->triggerBtnReadTime >= 100))) {
    this->isRatcheting = true;

    int ratchetAmountVal = map(this->ratchetPot.getValue(), 10, 1000, 100, 0);
    this->handleRatchet(ratchetAmountVal);
    
  } else if (this->triggerBtn.fell()) {
    this->currentInstrument->trigger(1);
    this->isRatcheting = false;
  }
}

void Kit::handleRatchet(byte value) {
  if (value > 0) {
    unsigned int ratchetAmoutMs = value;
    unsigned long timeSinceLastRatchetTriggered = (this->runtime - this->latestRatchetTriggerTime);
    bool firstPass = (timeSinceLastRatchetTriggered == this->runtime);
  
    if (firstPass || timeSinceLastRatchetTriggered >= ratchetAmoutMs) {
      this->currentInstrument->release(); // ensure prior triggers are released first
      this->currentInstrument->trigger(1);
      
      this->latestRatchetTriggerTime = this->runtime;
    }
  }

  this->stopRatchet();
}

void Kit::stopRatchet() {
  if (this->latestRatchetTriggerTime == 0) {
    return;
  }

  unsigned int releaseWindow = this->currentInstrument->releaseWindow;
  unsigned long timeSinceLastRatchetTriggered = (this->runtime - this->latestRatchetTriggerTime);
  unsigned long timeWithinRatchetRange = this->latestRatchetTriggerTime + releaseWindow;
  unsigned long ratchetBackOffTime = this->latestRatchetTriggerTime + releaseWindow + 70;

  if (timeSinceLastRatchetTriggered < releaseWindow) {
      releaseWindow = timeSinceLastRatchetTriggered + 50;
  }

  if (this->runtime == timeWithinRatchetRange) {
      this->currentInstrument->setQuickDecay(releaseWindow);
      this->currentInstrument->release();
  } else if (this->runtime == ratchetBackOffTime) {
      this->currentInstrument->resetDecay();
  }
}

void Kit::handleInstrumentReleases() {
  this->bassDrum->doRelease();
  this->snareDrum->doRelease();
  this->closedHat->doRelease();
  this->openHat->doRelease();
}

void Kit::setMixerLevel(int channel, byte value) {
  this->mix.gain(channel, (float)value * 0.01);
}
