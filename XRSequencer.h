#ifndef XRSequencer_h
#define XRSequencer_h

#include "Clock.h"
#include "Bounce2.h"
#include "XRConfig.h"
#include "XRKit.h"
//#include "XRSequencerButton.h"
#include "XRSequencerPattern.h"
#include "XRSequencerPatternCollection.h"
#include "XRSequencerPatternInstrumentTrack.h"
#include "XRSequencerPatternInstrumentTrackStep.h"

#include <vector>

class Sequencer {
   public:
    bool running = false;
    bool stepButtonPressed = false;
    bool patternSectionViewEnabled = false;
    bool patternSectionCursorEnabled = false;

    unsigned int currentPatternIndex = 0;
    unsigned int currentPatternTrackIndex = 0;
    unsigned int patternSectionViewCursorPosition = 0;
    
    Clock clock;

    Kit *kit;
    
    Button stepButtons[STEP_BUTTON_COUNT];
    Button functionButtons[FUNCTION_BUTTON_COUNT];
    Button transportButtons[TRANSPORT_BUTTON_COUNT];
    
    SequencerPattern *currentPattern;
    SequencerPatternInstrumentTrack *currentPatternTrack;

    std::vector<SequencerPattern *> patterns;
    SequencerPatternCollection *patternCollection;
    
    // Constructor
    Sequencer(
      Kit *kit,
      SequencerPatternCollection *patternCollection
    ) {
        this->kit = kit;
        this->patterns = patternCollection->getAll(); // fully loaded pattern collection (includes pattern tracks and steps)

        this->currentPattern = this->patterns.at(this->currentPatternIndex);
        this->currentPatternTrack = this->currentPattern->patternInstrumentTracks.at(this->currentPatternTrackIndex);
        
        this->setupInitialState();
    }

    bool isRunning();

    void updateState(); // main method. after reading inputs, this will update the state of the sequencer

   private:
    FunctionMode readiedMode = FunctionMode::PATTERN_PLAY;
    FunctionMode modeToResume = FunctionMode::PATTERN_PLAY;
    FunctionMode previousFunctionMode = FunctionMode::PATTERN_PLAY;
    FunctionMode functionMode = FunctionMode::PATTERN_PLAY;
    FunctionMode calculateFunctionMode();

    void readyPatternWriteMode();
    void leaveInstrumentSelectMode();

    void togglePatternSectionView();

    void setupInitialState();

    void setupFunctionLEDs();
    void doStepCounterLED(int step);
    void clearStepLEDs();
    void illuminateCurrentInstrumentLED();
    void setInitialLEDValuesForPatternSection();
    
    int getLEDStateForStep(int stepIdx);
    int getStepLEDState(int state);
    
    void setupButtons();
    void updateButtons();
    
    void doSetTriggerStates();
    void doInstrumentSelect();
    void doPatternGroupSelect();
    void doPatternSelect();
    void doPatternStep();
    void doSetTempo();
    void doStart();
    void doStopContinue();

    void start(bool reset);
    void stop();
};

#endif /* XRSequencer_h */
