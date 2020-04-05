#include "definitions.h"

#define S1 A0
#define S2 A1

int randomCounter;
long sensorCounterStart;

void setup()
{
  
  Serial.begin(115200);

  screenInit();

  pinMode(S1, INPUT);
  pinMode(S2, INPUT);
  sensorCounterStart = millis();
  randomCounter = 0;

  randomSeed(analogRead(3));

  
  Serial.println("Starting Screen test program...");
}

void loop() 
{
  if((millis() - sensorCounterStart) >= 10)
  {
    uint16_t s1DigitalRead = analogRead(S1);
    uint16_t s2DigitalRead = analogRead(S2);

    float s1Float = s1DigitalRead * (5.0/1024.0);
    float s2Float = s2DigitalRead * (5.0/1024.0);

    float fAvg = (s1Float + s2Float) / 2.0;

    
    setPGraphPoint(s1Float);
    setFGraphPoint(s2Float);
    setVGraphPoint(fAvg);

    if(randomCounter >= 30)
    {
      setPPeak(random(500,2000)/100.0);
      setPMean(random(500,2000)/100.0);
      setPEEP(random(500,2000)/100.0);
      setRR(random(500,2000)/100.0);
      setO2(random(500,2000)/100.0);
      setMVE(random(500,2000)/100.0);
      setVTI(random(500,2000)/100.0);
      setVTE(random(500,2000)/100.0);

      writeSRAMVar(9, SELECTION_E);

      randomCounter = 0;
    }
    else
    {
      randomCounter++;
    }
    
    sensorCounterStart = millis();
  }
  screenLoop();
}
