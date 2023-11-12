#include "Timer.h"
#include <toneAC.h>

const int led = 2;
const int piezoOutputPin = 3;
bool Time = false;
const int bpm = 80;
const int tempo = 60000 / bpm; // Set tempo to 80bpm
const int threshold = 5; // Variance in beat is 25ms
const int beat = tempo/2; // Set for 16th notes
int early = beat - threshold;
int late = beat + threshold;
int later = 50;
int earlier = 50;
Timer timer;
Timer timer1;

unsigned long previousMillis = 0;
unsigned long lastInputMillis = 0;

void setup() {
  pinMode(led, INPUT);
  pinMode(piezoOutputPin, OUTPUT);
  Serial.begin(9600);

}
void loop() {
  unsigned long currentMillis = millis(); 
  //Serial.println(currentMillis); 
  if (currentMillis - previousMillis >= tempo) {
    if (timer1.state() == RUNNING){ // Reset timer
      timer1.stop();
    }
    //Serial.println(currentMillis);
    timer1.start();
    previousMillis = currentMillis;
    toneAC(500); // Adjust the frequency based on your preference
    delay(10); // Adjust the delay based on the duration you want the sound to be ON
    toneAC();
    //Serial.println("Ryan sucks");
    lastInputMillis = currentMillis;
  }
  int pulse = digitalRead(led);
  if (timer.state() == STOPPED){ // Reset if timer times out
    Time = false;
  }
  if (pulse != 0){
    //Serial.print("High");
    unsigned long mill = millis();
    Serial.println(previousMillis);
    Serial.println(mill);
    if (Time == false){
      unsigned long startTime = millis();
      timer1.stop();
      int timed = timer1.read();
      unsigned long expectedTime = previousMillis + 5; // This is expected time for when pulse should go high
      //Serial.println(timed);
      //Serial.println(startTime - timed);
      //Serial.println(previousMillis);
      //Serial.println(0.025*tempo);
      Serial.println(expectedTime + later);
      Serial.println(expectedTime - earlier);
      if (startTime > (expectedTime + later) || startTime < (expectedTime - earlier)){ // If input is before threshold or after threshold, fail!!!
        Serial.println("Start on a downbeat!");
      }
      else{ // Otherwise start counting things
        timer.start();
        Serial.println("Start");
        Time = true;
      }

    }
    else if (Time == true){
      int startTime = millis();
      timer1.stop();
      int timed = timer1.read();
      timer.stop();
      int Delay = timer.read();
      if ((Delay > early) && (Delay < late)){
        Serial.println("On Time");
      }
      else if (Delay > late){
        Serial.println("Late");
      }
      else if (Delay < early){
        Serial.println("Early");
      }
      else {
        Serial.println("Wrong");
      }
      timer.start();
    }
    Serial.println(pulse);
    delay(110);
  }
  //Serial.println(pulse);
  //delay(110);
}

