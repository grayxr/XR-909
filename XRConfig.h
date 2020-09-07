#ifndef XRConfig_h
#define XRConfig_h

#include <ResponsiveAnalogRead.h>
#include <Bounce2.h>
#include <Arduino.h>

#define BAUD_RATE 9600

/**
 * Digital pins:
 * 
 * 0  - function button 1 (top)    LED
 * 1  - function button 2 (middle) LED
 * 2  - function button 3 (bottom) LED
 * 3  - trigger/instrument button  LED
 * 26 - START button               LED
 * 17 - STOP button                LED
 * 28 - trigger/instrument button  Switch
 * 4  - function button 3 (bottom) Switch
 * 30 - function button 2 (middle) Switch
 * 31 - function button 1 (top)    Switch
 * 33 - step button 4              Switch
 * 34 - step button 3              Switch
 * 36 - step button 2              Switch
 * 37 - step button 1              Switch
 * 35 - STOP button                Switch
 * 32 - START button               Switch
 * 
 * Analog pins:
 * 
 * A0   - step button 2             LED
 * A1   - step button 3             LED
 * A8   - step button 4             LED
 * A10  - step button 1             LED
 * A11  - trigger/ratchet           Pot
 * A14  - snappy                    Pot
 * A17  - tone                      Pot
 * A16  - tune                      Pot
 * A15  - level                     Pot
 * A13  - main volume               Pot
 * 
 * TX/RX pins:
 * 
 * TX7/29 - 7 Segment display
 * 
 */

// array size definitions

#define STEP_BUTTON_COUNT 4
#define FUNCTION_BUTTON_COUNT 3 // shift, instrument select, pattern group 1
#define TRANSPORT_BUTTON_COUNT 2 // start, stop/continue

#define STEP_LED_COUNT 4
#define FUNCTION_LED_COUNT 1 // pattern group 1
#define TRANSPORT_LED_COUNT 2 // start (prev), stop/continue (next)

// array index definitions

// Function
#define SHIFT_FUNCTION_BUTTON 0
#define INSTRUMENT_SELECT_FUNCTION_BUTTON 1
#define PATTERN_GROUP1_FUNCTION_BUTTON 2

#define PATTERN_GROUP1_FUNCTION_LED 0

// Transport
#define START_TRANSPORT_BUTTON 0
#define STOP_CONTINUE_TRANSPORT_BUTTON 1

#define START_TRANSPORT_LED 0
#define STOP_TRANSPORT_LED 1

// Step
#define STEP1_BUTTON 0
#define STEP2_BUTTON 1
#define STEP3_BUTTON 2
#define STEP4_BUTTON 3

#define STEP1_LED 0
#define STEP2_LED 1
#define STEP3_LED 2
#define STEP4_LED 3

static const int TUNEPOT = 40;
static const int LEVELPOT = 39;
static const int TONEPOT = 41;
static const int SNAPPYPOT = 38;
static const int RATCHETPOT = 25;
static const int MAINVOLPOT = 27;

static const int FUNCTION_BUTTON_PINS[FUNCTION_BUTTON_COUNT] { 4, 30, 31 };
static const int TRANSPORT_BUTTON_PINS[TRANSPORT_BUTTON_COUNT] { 32, 35 };
static const int STEP_BUTTON_PINS[STEP_BUTTON_COUNT] { 37, 36, 34, 33 };

static const int FUNCTION_LED_PINS[FUNCTION_LED_COUNT] { 0 };
static const int TRANSPORT_LED_PINS[TRANSPORT_LED_COUNT] { 26, 17 };
static const int STEP_LED_PINS[STEP_LED_COUNT] { A10, A0, A1, A8 };

enum class FunctionMode {
    START,
    STOP_CONTINUE,
    PATTERN_GROUP_SELECT,
    PATTERN_PLAY,
    PATTERN_SELECT,
    PATTERN_WRITE,
    INSTRUMENT_SELECT,
    SET_TEMPO,
    DEFAULT_MODE
};

#define INSTRUMENT_LIMIT 4 // eventually 11
#define PATTERN_LIMIT 4 // evenetually 16
#define PATTERN_INSTRUMENT_TRACK_LIMIT 4 // eventually 11
#define PATTERN_INSTRUMENT_TRACK_STEP_LIMIT 16
#define PATTERN_STEP_LIMIT 16

// #define DEBUG

#endif /* XRConfig_h */
