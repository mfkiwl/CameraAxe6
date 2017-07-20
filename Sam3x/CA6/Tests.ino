void caRunTests()
{
  //caTestSerialWritePerf();
  //caTestNetworkEcho();
  //caTestTickTimer();
  caTestPackets();
  //caTestBlinkLed();
  //caTestPerf();
  //caTestModulePorts();
  //caTestLinkAndCamPorts();
  //caTestAuxPort();
  //caTestEeprom();
  //caTestAnalog();
}

void caTestSerialWritePerf() {
  const uint32_t bufSize = 20;
  uint32_t startTime = 0;
  uint32_t endTime = 0;
  uint8_t writeChars[bufSize];

  Serial1.end();
  Serial1.begin(9600);

  // fill the write buffer
  for (uint8_t i = 0; i < bufSize; i++) {
    writeChars[i] = char((i % 10) + '0');
  }

  startTime = micros();
  Serial1.write(writeChars, bufSize);
  endTime = micros();
  delay(500); // Wait for values to finish being written

  CA_LOG("Serial Write perf (%d bytes): %d us\n", bufSize, endTime-startTime);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// caTestNetworkEcho - Tests performance of a round trip network
// returns  - NA
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void caTestNetworkEcho() {
  bool done = false;

  g_ctx.echoReceived = 0;

  uint32_t endTime;
  uint32_t startTime = millis();
  g_ctx.packetHelper.writePacketEcho(0, "01234");  // 0 means back to sam3x
  for(uint16_t i=0; i<5000; ++i) {
    processIncomingPacket();
    if (g_ctx.echoReceived) {
      endTime = millis();
      done = true;
      break;
    }
    delay(1);
  }

  if (done) {
    CA_LOG("Time in milliseconds: %d\n", endTime-startTime);
  } else {
    CA_LOG("ERROR - Packet was never returned.\n");
  }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// caTestTickTimer - Tests the CATickTimer class
// returns  - NA
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void caTestTickTimer()
{
  uint64_t ticks;
  CATickTimer bob(0);
  ticks = bob.convertTimeToTicks(0,0,0,100,0,0);
  bob.start(toggleLed, ticks, true);
  delay(5000);
  bob.stop();
}
void toggleLed()
{
  static uint8_t toggle = 1;

  hwPortPin led = CAU::getOnboardDevicePin(LED_GREEN_PIN);
  CAU::pinMode(led, OUTPUT);

  if (toggle)
  {
    CAU::digitalWrite(led, HIGH);
    toggle = 0;
  }
  else
  {
    CAU::digitalWrite(led, LOW);
    toggle = 1;
  }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// caTestPackets - Tests packing and unpacking of data packets
// returns  - NA
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void caTestPackets()
{
  uint8_t unpackType;
  uint16_t packSize, unpackSize;
  uint8_t data[512];
  uint8_t flags = 0;

  memset(data, 0, 512);
  CAPacket unpackBase(STATE_UNPACKER, data, 512);
  CAPacket packBase(STATE_PACKER, data, 512);

  { // STRING Packet Test
    CAPacketString unpack0(unpackBase);             // Update per type
    CAPacketString pack0(packBase);                 // Update per type
    pack0.set(1, flags, "This is a string");        // Update per type
    packSize = pack0.pack();
    unpackSize = unpackBase.unpackSize();
    unpackType = unpackBase.unpackType();
    unpack0.unpack();
    if (packSize != unpackSize ||
          unpackType != PID_STRING ||               // Update per type
          unpack0.getClientHostId() != 1 ||
          unpack0.getFlags() != flags ||
          strcmp(unpack0.getString(), "This is a string") != 0) {
      CA_LOG("ERROR - STRING test failed\n");
    }
  }

  {  // UINT32 Packet Test
    CAPacketUint32 unpack0(unpackBase);             // Update per type
    CAPacketUint32 pack0(packBase);                 // Update per type
    pack0.set(2, flags, 123);                       // Update per type
    packSize = pack0.pack();
    unpackSize = unpackBase.unpackSize();
    unpackType = unpackBase.unpackType();
    unpack0.unpack();
    if (packSize != unpackSize ||
        unpackType != PID_UINT32 ||                 // Update per type
        unpack0.getClientHostId() != 2 ||
        unpack0.getFlags() != flags ||
        unpack0.getValue() != 123) {
      CA_LOG("ERROR - UINT32 test failed\n");
    }
  }

  {  // TIME_BOX Packet Test
    CAPacketTimeBox unpack0(unpackBase);            // Update per type
    CAPacketTimeBox pack0(packBase);                // Update per type
    pack0.set(3, flags, 99, 59, 40, 999, 500, 400); // Update per type
    packSize = pack0.pack();
    unpackSize = unpackBase.unpackSize();
    unpackType = unpackBase.unpackType();
    unpack0.unpack();
    if (packSize != unpackSize ||
        unpackType != PID_TIME_BOX ||               // Update per type
        unpack0.getClientHostId() != 3 ||
        unpack0.getFlags() != flags ||
        unpack0.getHours() != 99 ||
        unpack0.getMinutes() != 59 ||
        unpack0.getSeconds() != 40 ||
        unpack0.getMilliseconds() != 999 ||
        unpack0.getMicroseconds() != 500 ||
        unpack0.getNanoseconds() != 400) 
        {
      CA_LOG("ERROR - TIME_BOX test failed\n");
    }
  }

  { // MENU_SELECT Packet Test
    CAPacketMenuSelect unpack0(unpackBase);         // Update per type
    CAPacketMenuSelect pack0(packBase);             // Update per type
    pack0.set(1, 23);                               // Update per type
    packSize = pack0.pack();
    unpackSize = unpackBase.unpackSize();
    unpackType = unpackBase.unpackType();
    unpack0.unpack();
    if (packSize != unpackSize ||
          unpackType != PID_MENU_SELECT ||          // Update per type
          unpack0.getMode() != 1 ||
          unpack0.getMenuNumber() != 23) {
      CA_LOG("ERROR - MENU_SELECT test failed\n");
    } 
  }

  { // MENU_LIST Packet Test
    CAPacketMenuList unpack0(unpackBase);           // Update per type
    CAPacketMenuList pack0(packBase);               // Update per type
    pack0.set(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, "menuList"); // Update per type
    packSize = pack0.pack();
    unpackSize = unpackBase.unpackSize();
    unpackType = unpackBase.unpackType();
    unpack0.unpack();
    if (packSize != unpackSize ||
          unpackType != PID_MENU_LIST ||            // Update per type
          unpack0.getMenuId() != 1 ||
          unpack0.getModuleId0() != 2 ||
          unpack0.getModuleMask0() != 3 ||
          unpack0.getModuleId1() != 4 ||
          unpack0.getModuleMask1() != 5 ||
          unpack0.getModuleId2() != 6 ||
          unpack0.getModuleMask2() != 7 ||
          unpack0.getModuleId3() != 8 ||
          unpack0.getModuleMask3() != 9 ||
          unpack0.getModuleTypeId0() != 10 ||
          unpack0.getModuleTypeMask0() != 11 ||
          unpack0.getModuleTypeId1() != 12 ||
          unpack0.getModuleTypeMask1() != 13 ||
          strcmp(unpack0.getMenuName(), "menuList") != 0) {
      CA_LOG("ERROR - MENU_LIST test failed\n");
    } 
  }

  { // MODULE_LIST Packet Test
    CAPacketModuleList unpack0(unpackBase);         // Update per type
    CAPacketModuleList pack0(packBase);             // Update per type
    pack0.set(33, 44, "moduleList");                // Update per type
    packSize = pack0.pack();
    unpackSize = unpackBase.unpackSize();
    unpackType = unpackBase.unpackType();
    unpack0.unpack();
    if (packSize != unpackSize ||
          unpackType != PID_MODULE_LIST ||          // Update per type
          unpack0.getModuleId() != 33 ||
          unpack0.getModuleTypeId() != 44 ||
          strcmp(unpack0.getModuleName(), "moduleList") != 0) {
      CA_LOG("ERROR - MODULE_LIST test failed\n");
    } 
  }
  
  { // LOGGER Packet Test
    CAPacketLogger unpack0(unpackBase);             // Update per type
    CAPacketLogger pack0(packBase);                 // Update per type
    pack0.set("This is a log");                     // Update per type
    packSize = pack0.pack();
    unpackSize = unpackBase.unpackSize();
    unpackType = unpackBase.unpackType();
    unpack0.unpack();
    if (packSize != unpackSize ||
          unpackType != PID_LOGGER ||               // Update per type
          strcmp(unpack0.getLog(), "This is a log") != 0) {
      CA_LOG("ERROR - LOG test failed\n");
    } 
  }

  { // CAM_STATE Packet Test
    CAPacketCamState unpack0(unpackBase);           // Update per type
    CAPacketCamState pack0(packBase);               // Update per type
    pack0.set(2, 0xc1, 0xf0);                       // Update per type
    packSize = pack0.pack();
    unpackSize = unpackBase.unpackSize();
    unpackType = unpackBase.unpackType();
    unpack0.unpack();
    if (packSize != unpackSize ||
          unpackType != PID_CAM_STATE ||            // Update per type
          unpack0.getMultiplier() != 2 ||
          unpack0.getFocus() != 0xc1 ||
          unpack0.getShutter() != 0xf0) {
      CA_LOG("ERROR - CAM_STATE test failed\n");
    } 
  }

  { // CAM_SETTINGS Packet Test
    CAPacketCamSettings unpack0(unpackBase);        // Update per type
    CAPacketCamSettings pack0(packBase);            // Update per type
    pack0.set(50, 1, 999, 59, 58, 998, 997, 996, 57, 56, 995, 994, 0xbe, 1, 5, 1, 40, 41, 900);  // Update per type
    packSize = pack0.pack();
    unpackSize = unpackBase.unpackSize();
    unpackType = unpackBase.unpackType();
    unpack0.unpack();
    if (packSize != unpackSize ||
          unpackType != PID_CAM_SETTINGS ||         // Update per type
          unpack0.getCamPortNumber() != 50 ||
          unpack0.getMode() != 1 ||
          unpack0.getDelayHours() != 999 ||
          unpack0.getDelayMinutes() != 59 ||
          unpack0.getDelaySeconds() != 58 ||
          unpack0.getDelayMilliseconds() != 998 ||
          unpack0.getDelayMicroseconds() != 997 ||
          unpack0.getDurationHours() != 996 ||
          unpack0.getDurationMinutes() != 57 ||
          unpack0.getDurationSeconds() != 56 ||
          unpack0.getDurationMilliseconds() != 995 ||
          unpack0.getDurationMicroseconds() != 994 ||
          unpack0.getSequencer() != 0xbe ||
          unpack0.getApplyIntervalometer() != 1 ||
          unpack0.getSmartPreview() != 5 ||
          unpack0.getMirrorLockupEnable() != 1 ||
          unpack0.getMirrorLockupMinutes() != 40 ||
          unpack0.getMirrorLockupSeconds() != 41 ||
          unpack0.getMirrorLockupMilliseconds() != 900 ) {
      CA_LOG("ERROR - CAM_SETTINGS test failed\n");
    } 
  }

  { // INTERVALOMETER Packet Test
    CAPacketIntervalometer unpack0(unpackBase);     // Update per type
    CAPacketIntervalometer pack0(packBase);         // Update per type
    pack0.set(900, 50, 51, 901, 902, 903, 52, 53, 904, 905, 9999); // Update per type
    packSize = pack0.pack();
    unpackSize = unpackBase.unpackSize();
    unpackType = unpackBase.unpackType();
    unpack0.unpack();
    if (packSize != unpackSize ||
          unpackType != PID_INTERVALOMETER ||       // Update per type
          unpack0.getStartHours() != 900 ||
          unpack0.getStartMinutes() != 50 ||
          unpack0.getStartSeconds() != 51 ||
          unpack0.getStartMilliseconds() != 901 ||
          unpack0.getStartMicroseconds() != 902 ||
          unpack0.getIntervalHours() != 903 ||
          unpack0.getIntervalMinutes() != 52 ||
          unpack0.getIntervalSeconds() != 53 ||
          unpack0.getIntervalMilliseconds() != 904 ||
          unpack0.getIntervalMicroseconds() != 905 ||
          unpack0.getRepeats() != 9999 ) {
      CA_LOG("ERROR - INTERVALOMETER test failed\n");
    } 
  }

  { // CONTROL_FLAGS Packet Test
    CAPacketControlFlags unpack0(unpackBase);       // Update per type
    CAPacketControlFlags pack0(packBase);           // Update per type
    pack0.set(1, 1);                                // Update per type
    packSize = pack0.pack();
    unpackSize = unpackBase.unpackSize();
    unpackType = unpackBase.unpackType();
    unpack0.unpack();
    if (packSize != unpackSize ||
          unpackType != PID_CONTROL_FLAGS ||        // Update per type
          unpack0.getSlaveModeEnable() != 1 ||
          unpack0.getExtraMessagesEnable() != 1 ) {
      CA_LOG("ERROR - CONTROL_FLAGS test failed\n");
    } 
  }

  { // ECHO Packet Test
    CAPacketEcho unpack0(unpackBase);               // Update per type
    CAPacketEcho pack0(packBase);                   // Update per type
    pack0.set(1, "Echo Packet");                    // Update per type
    packSize = pack0.pack();
    unpackSize = unpackBase.unpackSize();
    unpackType = unpackBase.unpackType();
    unpack0.unpack();
    if (packSize != unpackSize ||
          unpackType != PID_ECHO ||                 // Update per type
          unpack0.getMode() != 1 ||
          strcmp(unpack0.getString(), "Echo Packet") != 0) {
      CA_LOG("ERROR - ECHO test failed\n");
    } 
  }

  CA_LOG("Done - testPackets\n");
  delay(2000);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// caTestBlinkLed - Test blinks LEDs.
// returns  - NA
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void caTestBlinkLed()
{
  hwPortPin led = CAU::getOnboardDevicePin(LED_GREEN_PIN);
  CAU::pinMode(led, OUTPUT);
  CAU::digitalWrite(led, HIGH);
  delay(1000);
  CAU::digitalWrite(led, LOW);
  delay(200);

  led = CAU::getOnboardDevicePin(LED_RED_PIN);
  CAU::pinMode(led, OUTPUT);
  CAU::digitalWrite(led, HIGH);
  delay(200);
  CAU::digitalWrite(led, LOW);
  delay(200);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// caTestPerf - Various performance tests for low level CA6 functions
// returns  - NA
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void caTestPerf()
{
  uint16_t count = 0;
  uint8_t val8=0;
  uint16_t val16=0;
  uint32_t startTime, endTime;
  hwPortPin pp0 = CAU::getModulePin(0, 2); // actually Mod0_SDA
  
  startTime = micros();
  for(count=0; count<10000; ++count)
  {
    val8 = CAU::digitalRead(pp0);
  }
  endTime = micros();
  CA_LOG("  10,000 digitalRead() = %dus\n", endTime-startTime);

  startTime = micros();
  for(count=0; count<10000; ++count)
  {
    CAU::digitalWrite(pp0, LOW);
  }
  endTime = micros();
  CA_LOG("  10,000 digitalWrite() = %dus\n", endTime-startTime);

  pp0 = CAU::getOnboardDevicePin(LV_DETECT_PIN);
  CAU::initializeAnalog();
  CAU::pinMode(pp0, ANALOG_INPUT);
  startTime = micros();
  for(count=0; count<10000; ++count)
  {
    val16 = CAU::analogRead(pp0);
  }
  endTime = micros();
  CA_LOG("  10,000 analogRead() = %dus\n", endTime-startTime);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// caTestModulePorts - Tests all the module ports.  Reports which ones failed. - Requires a special test dongle.
// returns  - NA
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void caTestModulePorts()
{
  uint8_t module;
  uint8_t pin;

  for(module=0; module<4; ++module)
  {
    for(pin=0; pin<6; pin+=2)
    {
      hwPortPin pp0, pp1;
      pp0 = CAU::getModulePin(module, pin);
      pp1 = CAU::getModulePin(module, pin+1);
      if (!caTestTwoPins(pp0, pp1))
      {
        CA_LOG("  Module Port %d:%d failed\n", module, pin);
      }
      
      if (!caTestTwoPins(pp1, pp0))
      {
        CA_LOG("  Module Port %d:%d failed\n", module, pin+1);
      }
    }
  }
  CA_LOG("Done - module ports\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// caTestLinkAndCamPorts - Tests link and camera ports.  Reports which ones failed. - Requires a special test dongle.
// returns  - NA
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void caTestLinkAndCamPorts()
{
  hwPortPin ppFocus, ppShutter;
  uint8_t cam;
  hwPortPin linkFocus   = CAU::getLinkPin(FOCUS);
  hwPortPin linkShutter = CAU::getLinkPin(SHUTTER);
  uint8_t val0, val1, val2, val3, val4, val5;
  
  for(cam=0; cam<8; ++cam)
  {
    caAllPortsLow(); // Must reset all cam ports here
    CAU::pinMode(linkFocus,   INPUT);
    CAU::pinMode(linkShutter, INPUT);

    delayMicroseconds(90);  // Without this delay optoisolator trigger time causes failure
    val0 = CAU::digitalRead(linkFocus);    // Should be high due to pullup resistor
    val1 = CAU::digitalRead(linkShutter);  // Should be high due to pullup resistor
    ppFocus   = CAU::getCameraPin(cam, FOCUS);
    ppShutter = CAU::getCameraPin(cam, SHUTTER);
    CAU::digitalWrite(ppFocus,   HIGH);
    CAU::digitalWrite(ppShutter, LOW);
    delayMicroseconds(90);  // Without this delay optoisolator trigger time causes failure
    val2 = CAU::digitalRead(linkFocus);    // Should be low
    val3 = CAU::digitalRead(linkShutter);  // Should be high due to pullup resistor
    CAU::digitalWrite(ppFocus,   LOW);
    CAU::digitalWrite(ppShutter, HIGH);
    delayMicroseconds(90);  // Without this delay optoisolator trigger time causes failure
    val4 = CAU::digitalRead(linkFocus);    // Should be high due to pullup resistor
    val5 = CAU::digitalRead(linkShutter);  // Should be low
  
    if ((val0 != HIGH) || (val1 != HIGH) ||
        (val2 != LOW)  || (val3 != HIGH) ||
        (val4 != HIGH) || (val5 != LOW))
    {
      CA_LOG("  Camera/Link Port Cam:%d failed (%d,%d,%d,%d,%d,%d)\n", cam, val0, val1, val2, val3, val4, val5);
    }
  }
  
  CA_LOG("Done - camera/linkports\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// caTestLinkAndCamPorts - Tests the aux port.  Reports which pins failed. - Requires a special test dongle.
// returns  - NA
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void caTestAuxPort()
{
  uint8_t pin;
  hwPortPin pp0, pp1;

  for(pin=0; pin<46; pin+=2)
  {
    pp0 = CAU::getAuxPin(pin);
    pp1 = CAU::getAuxPin(pin+1);
    if (!caTestTwoPins(pp0, pp1))
    {
      CA_LOG("  Aux Port %d failed\n", pin);
    }
    if (!caTestTwoPins(pp1, pp0))
    {
      CA_LOG("  Aux Port %d failed\n", pin+1);
    }
  }
  
  CA_LOG("Done - aux port\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// caTestEeprom - Tests the eeprom
// returns  - NA
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void caTestEeprom()
{
  const char *str0 = "This is just a test string1.";
  const char *str1 = "Blah blah blah blah 2.";
  uint8_t buf[128];
  unioReadStatus statusReg;

  CAEeprom eeprom(CA_INTERNAL);
  
  if (!eeprom.statusWrite(EEPROM_WRITE_PROTECTED_ALL))
  {
    CA_LOG("  Writing status register FAILED\n");
  }
  if(!eeprom.statusRead(&statusReg))
  {
    CA_LOG("  Reading status register FAILED\n");
  }
  if (!eeprom.statusWrite(EEPROM_WRITE_PROTECTED_NONE))
  {
    CA_LOG("  Writing status register 2 FAILED\n");
  }

  if (!eeprom.write((uint8_t*)str0, 0x20, strlen(str0)+1))
  {
    CA_LOG("  Write1 FAILED\n");
  }
  memset(buf, 0, 128);
  if (!eeprom.read(buf, 0x20, strlen(str0)+1))
  {
    CA_LOG("  Read1 FAILED\n");
  }
  if (!eeprom.write((uint8_t*)str1, 0x20, strlen(str1)+1))
  {
    CA_LOG("  Write2 FAILED\n");
  }
  memset(buf, 0, 128);

  if (!eeprom.read(buf, 0x20, strlen(str1)+1))
  {
    CA_LOG("  Read2 FAILED\n");
  }

  CA_LOG("Done - eeprom\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// caTestAnalog - Tests the different analog pins
// returns  - NA
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void caTestAnalog()
{
  uint16_t val;
  hwPortPin ppAn;   // Analog Port
  hwPortPin ppDig;  // Port used to change analog's voltage level

  CAU::initializeAnalog();

  ppAn = CAU::getModulePin(0, 0);
  ppDig = CAU::getModulePin(0, 1);
  caTestAnalogPin(ppAn, ppDig);
  ppAn = CAU::getModulePin(0, 1);
  ppDig = CAU::getModulePin(0, 0);
  caTestAnalogPin(ppAn, ppDig);
  ppAn = CAU::getModulePin(0, 2);
  ppDig = CAU::getModulePin(0, 3);
  caTestAnalogPin(ppAn, ppDig);
  ppAn = CAU::getModulePin(1, 0);
  ppDig = CAU::getModulePin(1, 1);
  caTestAnalogPin(ppAn, ppDig);
  ppAn = CAU::getModulePin(2, 0);
  ppDig = CAU::getModulePin(2, 1);
  caTestAnalogPin(ppAn, ppDig);
  ppAn = CAU::getModulePin(2, 1);
  ppDig = CAU::getModulePin(2, 0);
  caTestAnalogPin(ppAn, ppDig);
  ppAn = CAU::getModulePin(2, 2);
  ppDig = CAU::getModulePin(2, 3);
  caTestAnalogPin(ppAn, ppDig);
  ppAn = CAU::getModulePin(2, 4);
  ppDig = CAU::getModulePin(2, 5);
  caTestAnalogPin(ppAn, ppDig);
  ppAn = CAU::getModulePin(2, 5);
  ppDig = CAU::getModulePin(2, 4);
  caTestAnalogPin(ppAn, ppDig);
  ppAn = CAU::getModulePin(3, 0);
  ppDig = CAU::getModulePin(3, 1);
  caTestAnalogPin(ppAn, ppDig);
  ppAn = CAU::getAuxPin(33);
  ppDig = CAU::getAuxPin(32);
  caTestAnalogPin(ppAn, ppDig);
  ppAn = CAU::getAuxPin(34);
  ppDig = CAU::getAuxPin(35);
  caTestAnalogPin(ppAn, ppDig);
  ppAn = CAU::getAuxPin(35);
  ppDig = CAU::getAuxPin(34);
  caTestAnalogPin(ppAn, ppDig);
  ppAn = CAU::getAuxPin(36);
  ppDig = CAU::getAuxPin(37);
  caTestAnalogPin(ppAn, ppDig);

  ppAn = CAU::getOnboardDevicePin(LV_DETECT_PIN);
  CAU::pinMode(ppAn, ANALOG_INPUT);
  val = CAU::analogRead(ppAn); // expected value is 5V/4 = 1.25V -- 1.25*4095/3.3 =1551
  if ((val < 1350) || (val > 1650))
  {
    CA_LOG("  Failed Analog Test -- LV_DETECT_PIN %d\n", val);
  }
  
  CA_LOG("Done - analog\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Test helper functions
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void caTestAnalogPin(hwPortPin ppAn, hwPortPin ppDig)
{
  uint16_t valLow, valHigh;

  caAllPortsLow(); // Helps detect shorted pins
  CAU::pinMode(ppAn, ANALOG_INPUT);
  CAU::pinMode(ppDig, OUTPUT);

  CAU::digitalWrite(ppDig, LOW);
  delayMicroseconds(20);  // I think this is needed due to capacitance in ribbon cable
  valLow = CAU::analogRead(ppAn);
  CAU::digitalWrite(ppDig, HIGH);
  delayMicroseconds(20);  // I think this is needed due to capacitance in ribbon cable
  valHigh = CAU::analogRead(ppAn);
  if (valLow >= 70 || valHigh <=4050)
  {
    CA_LOG("  Failed Analog Test pp(%d, %d) - %d %d\n", ppAn.port, ppAn.pin, valLow, valHigh);
  }
}

boolean caTestTwoPins(hwPortPin ppIn, hwPortPin ppOut)
{
  uint8_t val0, val1;
  boolean ret = true;

  caAllPortsLow(); // Helps detect shorted pins
  CAU::pinMode(ppOut, OUTPUT);
  CAU::digitalWrite(ppOut, LOW);
  CAU::pinMode(ppIn, INPUT);
  val0 = CAU::digitalRead(ppIn);
  CAU::digitalWrite(ppOut, HIGH);
  val1 = CAU::digitalRead(ppIn);
  if ((val0 != LOW) || (val1 != HIGH))
  {
    ret = false;
  }
  
  return ret;
}

// Set all the ports to output low
void caAllPortsLow()
{
  hwPortPin pp0;
  uint8_t i, j;

  // Aux Port
  for(i=0; i<46; ++i)
  {
    pp0 = CAU::getAuxPin(i);
    CAU::pinMode(pp0, OUTPUT);
    CAU::digitalWrite(pp0, LOW);
  }
  
  // Module Ports
  for(i=0; i<4; ++i)
  {
    for(j=0; j<6; ++j)
    {
      pp0 = CAU::getModulePin(i, j);
      CAU::pinMode(pp0, OUTPUT);
      CAU::digitalWrite(pp0, LOW);
    }
  }

  // Camera Ports
  for(i=0; i<8; ++i)
  {
    pp0 = CAU::getCameraPin(i, FOCUS);
    CAU::pinMode(pp0, OUTPUT);
    CAU::digitalWrite(pp0, LOW);
    pp0 = CAU::getCameraPin(i, SHUTTER);
    CAU::pinMode(pp0, OUTPUT);
    CAU::digitalWrite(pp0, LOW);
  }

  // Link Port
  pp0 = CAU::getLinkPin(FOCUS);
  CAU::pinMode(pp0, OUTPUT);
  CAU::digitalWrite(pp0, LOW);
  pp0 = CAU::getLinkPin(SHUTTER);
  CAU::pinMode(pp0, OUTPUT);
  CAU::digitalWrite(pp0, LOW);
}

