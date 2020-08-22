#include "XRKit.h"
#include <Arduino.h>

Kit::Kit(
  Bass *bass,
  Snare *snare
){
  this->bass = bass;
  this->snare = snare;
}

void Kit::init() {
  this->bass->init();
  this->snare->init();
}

void Kit::testKit() {
  testBass();
  delay(500);
  testSnare();
  delay(500);
  testBass();
  delay(500);
  testSnare();
  delay(500);
  testBoth();
  delay(500);
  testSnare();
  delay(500);
  testBass();
  delay(500);
  testSnare();
  delay(500);
}

void Kit::testBass() {
  if (this->bass->initialized == false) { 
    Serial.println("Bass drum not initialized!");
    return;
  }
  
  Serial.println("Bass drum test");
  
  this->bass->note = 32;
  this->bass->velocity = 127;
  this->bass->play(); 
  delay(100);
  this->bass->stop();
}

void Kit::testSnare() {
  if (this->snare->initialized == false) { 
    Serial.println("Snare drum not initialized!");
    return;
  }
  
  Serial.println("Snare drum test");
  
  this->snare->note = 56;
  this->snare->velocity = 127;
  this->snare->play(); 
  delay(100);
  this->snare->stop();
}

void Kit::testBoth() {  
  this->bass->note = 32;
  this->bass->velocity = 127;
  this->snare->note = 56;
  this->snare->velocity = 127;
  this->snare->play();
  this->bass->play(); 
  delay(100);
  this->snare->stop();
  this->bass->stop();
}
