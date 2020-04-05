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

void setState()
{
  if(settingState == 0)
  {
    Serial.println("Entering Setting State");
    settingState = 1;
    attachInterrupt(digitalPinToInterrupt(ENCODERA), encoderA, CHANGE);
  }
  else
  {
    Serial.println("Exiting Setting State");
    settingState = 0;
    detachInterrupt(digitalPinToInterrupt(ENCODERA));
  }
}

void setOpState()
{
  if(opState == 0)
  {
    Serial.println("Starting Dev Operation");
    opState = 1;
  }
  else if(opState == 1)
  {
    Serial.println("Ending Dev Operation");
    opState = 0; 
  }
}

bool checkIfNewSelectionValue()
{
  bool result;

  result = false;
  if(millis() - encWindowStart >= ENCWINDOW)
  {
    if(lastEncoderVal != encoder)
    {     
      selectionVal = selectionVal + encoder;
      selectionVal = selectionVal;
      
      if(selectionVal >= ENCODERMAXVAL && encDir == ENCDIRUP)
      {
        selectionVal = ENCODERMAXVAL;
        encoder = 0;
      }
      else if(selectionVal <= ENCODERMINVAL && encDir == ENCDIRDOWN)
      {
        selectionVal = ENCODERMINVAL;
        encoder = 0;
      }

      lastEncoderVal = encoder;
      result = true;
    }
    encWindowStart = millis();
  }

  return result;
}

bool checkEncButtonPressed()
{
  if(checkEncButton() == HIGH)
  {
    while(checkEncButton() == HIGH);
    delay(50);
    return true;
  }
  return false;
}

uint16_t getScreenButtonId ()
{
  uint16_t buttonId;
  
  buttonId = (screenRxBuffer[index-5] << 8) | screenRxBuffer[index-4];

  return buttonId;
}

bool checkIfScreenSet(uint16_t buttonId)
{
  if(buttonId == SCREEN_SETTING_ID)
  {
    return true;
  }
  return false;
}

bool checkIfScreenGo(uint16_t buttonId)
{
  if(buttonId == SCREEN_GO_ID)
  {
    return true;
  }
  return false;
}

int checkEncButton()
{
  return digitalRead(PUSHBUTTON);  
}


void encoderA()
{
  int aState;

  aState = digitalRead(ENCODERA);

  if(digitalRead(ENCODERB) != aState)
  {
    encoder++;
//    encoder = encoder/2;
    encDir = ENCDIRUP;
  }
  else
  {
    encoder--;
//    encoder = encoder/2;
    encDir = ENCDIRDOWN;
  }

//  Serial.print("Encoder: ");
//  Serial.println(encoder);
  delay(10);
}


bool checkIfScreenInput(long timeout, uint8_t* dataBuffer, uint16_t* index)
{
  long start;
  if(Serial1.available() > 0)
  {    
//    if((millis() - start) >= timeout)
//    {
//      return false;
//    }
    uint16_t i = 0;
    uint8_t endFlag = 0;
    uint8_t packetLen = 0;

    while(endFlag != 1)
    {
      if(Serial1.available() > 0)
      {
        dataBuffer[i] = Serial1.read();
        Serial.print("Byte received: ");
        Serial.println(dataBuffer[i], HEX);
        i++;
      }
      if(i == 3)
      {
        packetLen = dataBuffer[i-1];
        Serial.print("Packet length: ");
        Serial.println(packetLen);
      }
      else if(packetLen != 0 && i >= (packetLen + 3))
      {
        endFlag = 1;
      }
    }
    *index = i;
    return true;
  }
  return false;
}

void writeSRAMVar(uint16_t value, uint16_t pointer)
{
  uint8_t data[2];
  uint8_t pointerBytes[2];
  uint8_t txBuffer[8];
  uint8_t packetLen;

  packetLen = 5;
  
  pointerBytes[0] = (pointer >> 8) & 0xFF;
  pointerBytes[1] = pointer & 0xFF;

  data[0] = (value >> 8) & 0xFF;
  data[1] = value & 0xFF;

  txBuffer[0] = 0x5A;
  txBuffer[1] = 0xA5;
  txBuffer[2] = packetLen;
  txBuffer[3] = 0x82; //Command to write SRAM Variable
  txBuffer[4] = pointerBytes[0];
  txBuffer[5] = pointerBytes[1];
  txBuffer[6] = data[0];
  txBuffer[7] = data[1];

  sendToScreen(txBuffer, 8);
}

void readSRAMVar(uint16_t pointer, uint8_t wordLen, uint16_t* returnBuffer)
{
  uint8_t pointerBytes[2];
  uint8_t txBuffer[7];
  uint8_t packetLen;
  uint8_t rxBuffer[wordLen * 2];
  uint8_t rxIndex;
  uint16_t regWord[wordLen];
  

  pointerBytes[0] = (pointer >> 8) & 0xFF;
  pointerBytes[1] = pointer & 0xFF;

  txBuffer[0] = 0x5A;
  txBuffer[1] = 0xA5;
  txBuffer[2] = packetLen;
  txBuffer[3] = 0x83; //Command to read SRAM Variable
  txBuffer[4] = pointerBytes[0];
  txBuffer[5] = pointerBytes[1];
  txBuffer[6] = wordLen;

  sendToScreen(txBuffer,7);

  rxIndex = 0;
  
  while(Serial1.available() > 0)
  {
    rxBuffer[rxIndex] = Serial1.read();
    rxIndex++;
  }

  for(uint16_t i=0; i<wordLen; i++)
  {
    returnBuffer[i] = (rxBuffer[2*i] << 8) | rxBuffer[2*i + 1];
  }
}

void writeRTGSingle(uint16_t datapoint, uint8_t channel)
{
  uint8_t data[2];
  uint8_t txBuffer[7];
  uint8_t packetLen;

  packetLen = 4;  //Fixed for single datapoint in single real time graph

  data[0] = (datapoint >> 8) & 0xFF;
  data[1] = datapoint & 0xFF;

  txBuffer[0] = 0x5A;
  txBuffer[1] = 0xA5;
  txBuffer[2] = packetLen;
  txBuffer[3] = 0x84; //Command for Dynamic Trend Curve
  txBuffer[4] = channel;
  txBuffer[5] = data[0];
  txBuffer[6] = data[1];

  sendToScreen(txBuffer,7);
}

void writeRTGSMulti(uint16_t* datapoints, uint8_t dpSize, uint8_t channels)
{
  uint16_t txBufferLen = 5 + 2 * dpSize;
  uint8_t convDataBuffer[2 * dpSize];
  uint8_t txBuffer[txBufferLen];
  uint8_t packetLen;

  packetLen = 2 + 2 * dpSize;

  for(int i=0; i<dpSize; i++)
  {
    convDataBuffer[2*i] = (datapoints[i] >> 8) & 0xFF;
    convDataBuffer[2*i + 1] = datapoints[i] & 0xFF;
  }

  txBuffer[0] = 0x5A;
  txBuffer[1] = 0xA5;
  txBuffer[2] = packetLen;
  txBuffer[3] = 0x84;
  txBuffer[4] = channels;
  for(int i=5; i<(5 + 2*dpSize); i++)
  {
    txBuffer[i] = convDataBuffer[i-5];
  }

  sendToScreen(txBuffer, txBufferLen);
}

void sendToScreen(uint8_t* dataBuffer,uint16_t bufferSize)
{
  for(uint16_t i=0; i<bufferSize; i++)
  {
    Serial1.write(dataBuffer[i]);
  }
}
