void screenInit()
{
  Serial1.begin(115200);

//  attachInterrupt(digitalPinToInterrupt(ENCODERA), encoderA, CHANGE);

  pinMode(SENSOR1, INPUT);
  pinMode(PUSHBUTTON, INPUT);
  pinMode(ENCODERA, INPUT);
  pinMode(ENCODERB, INPUT);
  
  encoder = 0;
  lastEncoderVal = 0;
  selectionVal = 0;
  settingState = 0;
  opState = 0;

  encWindowStart = millis();
}

void screenLoop()
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
      writeSRAMVar(selectionVal, SELECTION_I);
    }
  }
}

void setPPeak(float value)
{
  uint16_t ppeakValue = value;
  
  writeSRAMVar(ppeakValue, PPEAK);
}

void setPMean(float value)
{
  uint16_t pmeanValue = value;
  
  writeSRAMVar(pmeanValue, PMEAN);
}

void setPEEP(float value)
{
  uint16_t peepValue = value;

  writeSRAMVar(peepValue, PEEP);
}

void setRR(float value)
{
  uint16_t rrValue = value;

  writeSRAMVar(rrValue, RR);
}

void setO2(float value)
{
  uint16_t o2Value = value;

  writeSRAMVar(o2Value, O2);
}

void setMVE(float value)
{
  uint16_t mveValue = value;

  writeSRAMVar(mveValue, MVE);
}

void setVTI(float value)
{
  uint16_t vtiValue = value;

  writeSRAMVar(vtiValue, VTI);
}

void setVTE(float value)
{
  uint16_t vteValue = value;

  writeSRAMVar(vteValue, VTE);
}

void setPGraphPoint(float datapoint)
{
  uint16_t digital;

  digital = (uint16_t)((PGRAPHRES/PMAXVAL) * datapoint);
  writeRTGSingle(digital, PGRAPH_ID);
}

void setFGraphPoint(float datapoint)
{
  uint16_t digital;

  digital = (uint16_t)((FGRAPHRES/FMAXVAL) * datapoint);
  writeRTGSingle(digital, FGRAPH_ID);
}

void setVGraphPoint(float datapoint)
{
  uint16_t digital;

  digital = (uint16_t)((VGRAPHRES/VMAXVAL) * datapoint);
  writeRTGSingle(digital, VGRAPH_ID);
}
