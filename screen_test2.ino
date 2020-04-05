#include "definitions.h"

void setup()
{
  
  Serial.begin(115200);

  screenInit();
  
  Serial.println("Starting Screen test program...");
}

void loop() 
{
    if(checkIfScreenInput(50, screenRxBuffer, &index))
    {
      Serial.println("Screen touched");
      uint16_t bId = getScreenButtonId();
      if(checkIfScreenSet(bId))
      {
        setState();
      }
      else if(checkIfScreenGo(bId))
      {
        setOpState();
      }
    }

    if(checkEncButtonPressed())
    {
      setState();
    }

    if(settingState == 1)
    {
      if(checkIfNewSelectionValue())
      {
        Serial.print("Selected Value: ");
        Serial.println(selectionVal);
      }
    }
    
//    sensor1Val = analogRead(SENSOR1);
//    Serial.print("Sensor Value is: ");
//    Serial.println(sensor1Val);
}
