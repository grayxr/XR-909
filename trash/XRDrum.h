#include <Arduino.h>

class Drum {
  public:
   virtual void play();
   // Needs to be implemented by each subclass
   
   virtual void stop();
   // Needs to be implemented by each subclass
   
   virtual void noteOnHandler(byte channel, byte note, byte velocity) = 0;
   // Needs to be implemented by each subclass
   
   virtual void noteOffHandler(byte channel, byte note, byte velocity) = 0;
   // Needs to be implemented by each subclass

   virtual void ccHandler(byte channel, byte control, byte value) = 0;
   // Needs to be implemented by each subclass
   
   virtual ~Drum() {}
   // Destructor
};
