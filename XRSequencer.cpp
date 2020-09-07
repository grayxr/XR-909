#include "XRSequencer.h"
#include "XRSequencerPattern.h"
#include "XRSequencerPatternInstrumentTrack.h"
#include "XRSequencerPatternInstrumentTrackStep.h"

#include "XRConfig.h"

#include <Arduino.h>
#include <sstream>
#include <algorithm> // for std::find
#include <iterator> // for std::begin, std::end

void Sequencer::setupInitialState() {
    Serial.println("setting up initial state of sequencer...");
    this->setupButtons();
    this->doSetTempo(); // initial tempo of 120
}

bool Sequencer::isRunning() { 
  return this->running; 
}

// Triggers the pattern step and increments the current pattern step by one
// TODO: ensure that the current measure is completed before advancing to the next/target pattern
void Sequencer::doPatternStep() {
    this->currentPattern->doStep(this->kit);
}

void Sequencer::start(bool reset = true) {
    if (this->running == false) {
        this->running = true;

        if (reset) {
          this->currentPattern->onStart();
        }
        
        clock.onStart();
        Serial.println("starting!");
    }
}

// stop/continue
void Sequencer::stop() {
    if (this->running) {
        this->running = false;
        
        clock.onStop();
        Serial.println("stopping!");
    } else {
        this->start(false);
        Serial.println("continuing!");
    }
}

void Sequencer::readyPatternWriteMode() {
  if (readiedMode != FunctionMode::PATTERN_WRITE) {
      Serial.println("Time to ready pattern write mode!");
      Serial.println("1. The pattern group 1 button should be blinking!");

      this->patternSectionViewEnabled = true;
      
      readiedMode = FunctionMode::PATTERN_WRITE;
  }
}

void Sequencer::doInstrumentSelect() {
  // for any key button, if it is pressed, then that instrument was selected
  // so selected pattern instrument track should be made ready for write
  // then, leave this mode, resume the previous one.

  if (previousFunctionMode != FunctionMode::INSTRUMENT_SELECT) {
    this->clearStepLEDs();
    this->illuminateCurrentInstrumentLED();
  }
  
  for (unsigned int i = 0; i < STEP_BUTTON_COUNT; ++i) {    
    if (stepButtons[i].pressed()) {
        Serial.print("instrument selected: ");
        Serial.println(i);
        this->currentPatternTrackIndex = i;
        this->currentPatternTrack = this->currentPattern->patternInstrumentTracks.at(i);

        this->kit->setCurrentInstrumentByTrack(i);
        
        this->clearStepLEDs();
        this->illuminateCurrentInstrumentLED();
    }
  }
}

void Sequencer::togglePatternSectionView() {
    if (this->patternSectionViewEnabled == false) {
        this->patternSectionViewEnabled = true;
    } else {
        this->patternSectionViewEnabled = false;
    }
}

void Sequencer::updateState() {  
    this->updateButtons();
    
    functionMode = calculateFunctionMode();
    
    if (functionMode == FunctionMode::PATTERN_PLAY){
        for (unsigned int i = 0; i < STEP_BUTTON_COUNT; ++i) {
            if (stepButtons[i].pressed()) {
                Serial.println("changing pattern!");
                
//                this->currentPatternIndex = i;
//                this->currentPatternTrackIndex = 0;
//          
//                this->currentPattern = this->patterns.at(this->currentPatternIndex);
//                this->currentPatternTrack = this->currentPattern->patternInstrumentTracks.at(this->currentPatternTrackIndex);
            }
        }
    }

//    if (this->patternSectionViewEnabled && functionButtons[SHIFT_FUNCTION_BUTTON].read() == false) {
//        if (transportButtons[START_TRANSPORT_BUTTON].rose()) {
//            Serial.println("pattern section view cursor backward!");
//            this->patternSectionCursorEnabled = true;
//            if (this->patternSectionViewCursorPosition > 0) {
//              --this->patternSectionViewCursorPosition;
//            }
//        } else if (transportButtons[STOP_CONTINUE_TRANSPORT_BUTTON].rose()) {
//            Serial.println("pattern section view cursor forward!");
//            this->patternSectionCursorEnabled = true;
//            if (this->patternSectionViewCursorPosition < 4) {
//              ++this->patternSectionViewCursorPosition;
//            }
//        }
//    }

    switch (functionMode) {
        case FunctionMode::START:
            functionMode = previousFunctionMode;
            this->start();
            break;
        case FunctionMode::STOP_CONTINUE:
            this->stop();
            functionMode = previousFunctionMode;
            break;
        case FunctionMode::SET_TEMPO:
            this->doSetTempo();
            break;
        case FunctionMode::PATTERN_GROUP_SELECT:
            //  TODO ...
            break;
        case FunctionMode::PATTERN_WRITE:
            this->readyPatternWriteMode();
            break;
        case FunctionMode::INSTRUMENT_SELECT:
            this->doInstrumentSelect();
            break;
        default:
            break;
    }

    // set step triggers before we do the step, so patterns can be modified before they reset  
    if (functionMode == FunctionMode::PATTERN_WRITE) { // add other modes where this is supported
        // handle step button presses
        this->doSetTriggerStates();
    }

    // update clock and see if its time for the next step.
    bool step = clock.update();

    // indicate current step / illuminate current step key (NOTE: this is the fast scrolling lights as the sequence plays)
    if (functionMode == FunctionMode::PATTERN_PLAY || functionMode == FunctionMode::PATTERN_WRITE) {      
        if (running && step) {
            this->doStepCounterLED(this->currentPattern->currentPatternStepIndex);
        }
    }
    
    if (running) {
        // check if we should step forward in the pattern (and in track mode too, eventually)
        if (step){
//            Serial.print("current pattern track: ");
//            Serial.print(this->currentPatternTrackIndex);
//            Serial.print(" current step: ");
//            Serial.println(this->currentPattern->currentPatternStepIndex);
            this->doPatternStep();
        }
    }

    // track function mode changes on each loop
    previousFunctionMode = functionMode;
}

/**
 * BUTTON presses:
 * rose() -> button was pressed down
 * read() -> button is currently pressed down
 * fell() -> button was released
 */
FunctionMode Sequencer::calculateFunctionMode() {
    // check for functions based on combos (shift + other button.read(), etc)
    if (
      functionMode == FunctionMode::PATTERN_WRITE
      && functionButtons[INSTRUMENT_SELECT_FUNCTION_BUTTON].read()
    ){
        Serial.println("Select the instrument!");
        return FunctionMode::INSTRUMENT_SELECT;
    } else if (
      functionMode == FunctionMode::INSTRUMENT_SELECT
      && functionButtons[INSTRUMENT_SELECT_FUNCTION_BUTTON].read() == false
    ){
        Serial.println("Left instrument selection!");
        return FunctionMode::PATTERN_WRITE; // TODO: implement modeToResume
    } else if (functionButtons[SHIFT_FUNCTION_BUTTON].read()) {
        if (
            functionMode == FunctionMode::PATTERN_WRITE
            && transportButtons[START_TRANSPORT_BUTTON].read() == false
            && transportButtons[STOP_CONTINUE_TRANSPORT_BUTTON].read() == false
        ){
            Serial.println("toggle pattern section view!");
            this->togglePatternSectionView();
        }
        
        if (functionMode != FunctionMode::PATTERN_WRITE && functionButtons[PATTERN_GROUP1_FUNCTION_BUTTON].pressed()) {
            Serial.println("Pattern write mode enabled!");
            return FunctionMode::PATTERN_WRITE;
        }
    } else {
      //if (this->patternSectionViewEnabled == false) {
          if (transportButtons[START_TRANSPORT_BUTTON].pressed()) {
              Serial.println("start!");
              return FunctionMode::START;
          }
          if (transportButtons[STOP_CONTINUE_TRANSPORT_BUTTON].pressed()) {
              Serial.println("stop / continue!");
              return FunctionMode::STOP_CONTINUE;
          }
      //}
      
//      if (functionButtons[PATTERN_GROUP1_FUNCTION_BUTTON].pressed()) {
//          Serial.println("pattern group select!");
//          return FunctionMode::PATTERN_GROUP_SELECT;
//      }
    }
    
    return functionMode;
}

void Sequencer::doSetTempo(){
    // default tempo behavior
    clock.setClockMode(ClockMode::INTERNAL_CLOCK);

    // TODO: accept tempo input
    // clock.setClockMode(ClockMode::INTERNAL_CLOCK);
    // clock.setStepLength(map(rawValue, 0, 1024, 512000, 32000));
    
    int tempTempo = 800; // (60 seconds / 120 beats) = 0.5 seconds
    
    clock.setStepLength(map(tempTempo, 0, 1024, 512000, 32000));
}

/*
 * Default mode (no mode button pressed). Checks for step button presses and
 * translates presses to trigger state changes.
 * 
 * TODO: copy/paste operation
 */
void Sequencer::doSetTriggerStates() {      
    for (unsigned int i = 0; i < STEP_BUTTON_COUNT; i++) {
        Button btn = stepButtons[i];
        int section = this->currentPattern->currentPatternSectionIndex;
        int correctedStepIndex = i;

        if (btn.pressed()) {

            // make button position correlate to specific key step position, not the global current step position
            
            if (section ==  1) {
                correctedStepIndex = i + 4;
            } else if (section == 2) {
                correctedStepIndex = i + 8;
            } else if (section == 3) {
                correctedStepIndex = i + 12;
            }

        Serial.print("button key index: ");
        Serial.print(i);
        Serial.print(" pattern section: ");
        Serial.print(section);
        Serial.print(" corrected step index for this trigger: ");
        Serial.println(correctedStepIndex);
        
            SequencerPatternInstrumentTrackStep *stepIter = this->currentPatternTrack->getInstrumentTrackStepAtIndex(correctedStepIndex);
            
            // toggle the step on/accent/off
            int newState = stepIter->toggleTriggerState();

            // update the step LED
            switch(newState) {
                case 0:
                  analogWrite(STEP_LED_PINS[i], 0);
                  break;
                case 1:
                  analogWrite(STEP_LED_PINS[i], 5);
                  break;
                case 2:
                  analogWrite(STEP_LED_PINS[i], 20);
                  break;
            }
        }
    }
}

void Sequencer::setupButtons() {
    for (unsigned int i = 0; i < FUNCTION_BUTTON_COUNT; ++i) {
        functionButtons[i] = Button();
        functionButtons[i].attach(FUNCTION_BUTTON_PINS[i], INPUT_PULLDOWN);
        functionButtons[i].interval(25);
        functionButtons[i].setPressedState(LOW);
    }   
    for (unsigned int i = 0; i < TRANSPORT_BUTTON_COUNT; ++i) {
        transportButtons[i] = Button();
        transportButtons[i].attach(TRANSPORT_BUTTON_PINS[i], INPUT_PULLUP);
        transportButtons[i].interval(25);
        transportButtons[i].setPressedState(LOW);
    }
    for (unsigned int i = 0; i < STEP_BUTTON_COUNT; ++i) {
        stepButtons[i] = Button();
        stepButtons[i].attach(STEP_BUTTON_PINS[i], INPUT_PULLUP);
        stepButtons[i].interval(25);
        stepButtons[i].setPressedState(LOW);
    }
    // TODO: set this up in KIT instead
//    for (unsigned int i = 0; i < TRIGGER_BUTTON_COUNT; ++i) {
//        instrumentButtons[i] = Button();
//        instrumentButtons[i].interval(5);
//        instrumentButtons[i].attach(TRIGGER_BUTTON_PINS[i]);
//        instrumentButtons[i].setPressedState(LOW);
//    }
}

void Sequencer::updateButtons() {
    for (unsigned int i = 0; i < STEP_BUTTON_COUNT; ++i) {
        stepButtons[i].update();
    }
    for (unsigned int i = 0; i < FUNCTION_BUTTON_COUNT; ++i) {
        functionButtons[i].update();
    }
    for (unsigned int i = 0; i < TRANSPORT_BUTTON_COUNT; ++i) {
        transportButtons[i].update();
    }
    // TODO: set this up in KIT instead
//    for (unsigned int i = 0; i < TRIGGER_BUTTON_COUNT; ++i) {
//        instrumentButtons[i].update();
//    }
}

void Sequencer::setupFunctionLEDs() {
    for (unsigned int i = 0; i < FUNCTION_LED_COUNT; ++i) {
        pinMode(FUNCTION_LED_PINS[i], OUTPUT);
        digitalWrite(FUNCTION_LED_PINS[i], LOW);
    }
}

void Sequencer::clearStepLEDs() {
    for (unsigned int i=0; i < STEP_LED_COUNT; ++i) {
        analogWrite(STEP_LED_PINS[i], 0);
    }
}

int Sequencer::getLEDStateForStep(int stepIdx) {
    SequencerPatternInstrumentTrackStep *step = this->currentPatternTrack->patternInstrumentTrackSteps.at(stepIdx);

    return this->getStepLEDState(step->getState());
}

int Sequencer::getStepLEDState(int state) {
    int LEDState = 0;

    if (state == 1) {
        LEDState = 5;
    } else if (state == 2) {
        LEDState = 20;
    }
  
    return LEDState;
}

void Sequencer::doStepCounterLED(int step) {
    // TODO: for when we have 16 step keys
    // analogWrite(STEP_LED_PINS[this->currentPattern->currentPatternStepIndex], 255);

    if (this->patternSectionViewEnabled) {
        this->setInitialLEDValuesForPatternSection();
    }

    // for when we have only 4 step keys
    if (functionMode == FunctionMode::PATTERN_PLAY && this->patternSectionViewEnabled == false) {         
          if (step <= 3) {
              analogWrite(STEP_LED_PINS[3], 0);
              analogWrite(STEP_LED_PINS[0], 20);
          } else if (step >= 4 && step <= 7) {
              analogWrite(STEP_LED_PINS[0], 0);
              analogWrite(STEP_LED_PINS[1], 20);
          } else if (step >= 8 && step <= 11) {
              analogWrite(STEP_LED_PINS[1], 0);
              analogWrite(STEP_LED_PINS[2], 20);
          } else if (step >= 12 && step <= 15) {
              analogWrite(STEP_LED_PINS[2], 0);
              analogWrite(STEP_LED_PINS[3], 20);
          }
      } else if (this->patternSectionViewEnabled && this->patternSectionCursorEnabled == false) {
          
          if (step <= 3) {
            
              if (step == 0) {
                analogWrite(STEP_LED_PINS[3], getLEDStateForStep(3));
              } else {
                analogWrite(STEP_LED_PINS[step-1], getLEDStateForStep(step-1));
              }
              
              analogWrite(STEP_LED_PINS[step], 20);
              
          } else if (step >= 4 && step <= 7) {
            
              if (step == 4) {
                analogWrite(STEP_LED_PINS[3], getLEDStateForStep(7));
              } else {
                analogWrite(STEP_LED_PINS[(step - 4)-1], getLEDStateForStep(step-1));
              }

              analogWrite(STEP_LED_PINS[step - 4], 20);
              
          } else if (step >= 8 && step <= 11) {
            
            if (step == 8) {
              analogWrite(STEP_LED_PINS[3], getLEDStateForStep(11));
            } else {
              analogWrite(STEP_LED_PINS[(step - 8)-1], getLEDStateForStep(step-1));
            }

              analogWrite(STEP_LED_PINS[step - 8], 20);
              
          } else if (step >= 12 && step <= 15) {
            
            if (step == 12) {
              analogWrite(STEP_LED_PINS[3], getLEDStateForStep(15));
            } else {
              analogWrite(STEP_LED_PINS[(step - 12)-1], getLEDStateForStep(step-1));
            }

              analogWrite(STEP_LED_PINS[step - 12], 20);
              
          }
      } else if (this->patternSectionViewEnabled && this->patternSectionCursorEnabled) {
          if (
              (this->patternSectionViewCursorPosition == 0 && step <= 4)
              || (this->patternSectionViewCursorPosition == 1 && step >= 5 && step <= 8)
              || (this->patternSectionViewCursorPosition == 2 && step >= 9 && step <= 12)
              || (this->patternSectionViewCursorPosition == 3 && step >= 13 && step <= 16)
          ){
              analogWrite(STEP_LED_PINS[step], 20);
          }
      }
}

void Sequencer::setInitialLEDValuesForPatternSection() {
    int section = this->currentPattern->currentPatternSectionIndex;

    for (auto step : this->currentPatternTrack->patternInstrumentTrackSteps) {
      int currStep = step->patternTrackStepIndex;
      int currStepSection2 = (currStep - 4);
      int currStepSection3 = (currStep - 8);
      int currStepSection4 = (currStep - 12);
      
      if (section == 0 && currStep < 4) {
          analogWrite(STEP_LED_PINS[currStep], getLEDStateForStep(currStep));
      } else if (section == 1 && currStep >= 4 && currStep < 8) {
          analogWrite(STEP_LED_PINS[currStepSection2], getLEDStateForStep(currStep));
      } else if (section == 2 && currStep >= 8 && currStep < 12) {
          analogWrite(STEP_LED_PINS[currStepSection3], getLEDStateForStep(currStep));
      } else if (section == 3 && currStep >= 12 && currStep < 16) {
          analogWrite(STEP_LED_PINS[currStepSection4], getLEDStateForStep(currStep));
      }
    }
}

void Sequencer::illuminateCurrentInstrumentLED() {
    analogWrite(STEP_LED_PINS[this->currentPatternTrackIndex], 20);
}
