////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Dreaming Robots - Copyright 2017, 2018
//
// Functions used to setup and trigger cameras/flashes (most of the triggering of cameras/flashes uses interrupts)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef TRIGGER_CAM_H
#define TRIGGER_CAM_H

extern void triggerCamerasPhase2();
extern void triggerCamerasPhase3();
extern void triggerCamerasPhase4();

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// camTriggerRunning() - Checks to see if we are currently in a trigger
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool camTriggerRunning() {
  return g_ctx.camTriggerRunning;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// triggerCameras() - This is the first of a 4 phase triggering process.  This function handles the
//  intervalometer start delay if it exists.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void triggerCameras() {
  if (camTriggerRunning()) {
    return;
  }

  g_ctx.camTriggerRunning = true;

  g_ctx.intervalometerCurRepeats = g_ctx.intervalometerRepeats;
  
  if (g_ctx.intervalometerEnable && (g_ctx.intervalometerStartTime!= 0)) {
    g_ctx.camTimer.start(triggerCamerasPhase2, g_ctx.intervalometerStartTime, true);
  }
  else {
    triggerCamerasPhase2();
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// triggerCamerasPhase2() - This is the second of a 4 phase triggering process.  This function handles any initial delay
//  before triggering (this keeps the next phase as simple as possible).
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void triggerCamerasPhase2() {
  g_ctx.curCamElement = 0;

  if (g_ctx.numCamElements == 0) {
    CA_LOG(CA_INFO, "No cameras triggered.\n");
    triggerCamerasPhase4();
  }

  // Trigger anything in this sorted list with 0 time
  if (g_ctx.camElements[g_ctx.curCamElement].timeOffset == 0) {
    triggerCamerasPhase3();
  }
  else {
    g_ctx.camTimer.start(triggerCamerasPhase3, g_ctx.camElements[g_ctx.curCamElement].timeOffset, true);
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// triggerCamerasPhase3() - This is the third of a 4 phase triggering process.  This function actually handles the triggering
//  of the cameras.  It walks through an a pre-generated array of camera elements.  Triggers one and then 
//  waits in a timer interrupt until the next camera element needs to be processed.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void triggerCamerasPhase3() {
  CamElement1& x = g_ctx.camElements[g_ctx.curCamElement];
  REG_PIOB_SODR = x.setMask & g_ctx.seqMask[g_ctx.sequencerValue];
  REG_PIOB_CODR = x.clearMask & g_ctx.seqMask[g_ctx.sequencerValue];

  if (++g_ctx.curCamElement == g_ctx.numCamElements) {
    triggerCamerasPhase4();
    return;
  }

  g_ctx.camTimer.start(triggerCamerasPhase3, g_ctx.camElements[g_ctx.curCamElement].timeOffset, true);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// triggerCamerasPhase4() - This is the forth of a 4 phase triggering process.  This function just handles the book keeping
//  needed to move to the next trigger event.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void triggerCamerasPhase4() {
  if (g_ctx.curSeqMask) {
    uint8_t b = g_ctx.curSeqBit;
    
    do {  // Shift the bit until we will trigger at least one camera/flash by checking sequencer mask
      b = (b << 1) | (b >> 7); //Shift bit left with wrap around
      g_ctx.sequencerValue = (g_ctx.sequencerValue+1 > NUM_SEQUENCER_BITS-1) ? 0 : (g_ctx.sequencerValue+1);
    } while ((b & g_ctx.curSeqMask) == 0);
    g_ctx.curSeqBit = b;
  }

  if (g_ctx.intervalometerEnable) {
    bool repeat = false;
  
    if (g_ctx.intervalometerRepeats == 0) {
      repeat = true;  //repeat forever
    }
    else if ((--g_ctx.intervalometerCurRepeats) > 0) {
      repeat = true;  // repeat until current repeats is zero
    }

    if (repeat) {
      if (g_ctx.intervalometerIntervalTime) {
        g_ctx.camTimer.start(triggerCamerasPhase2, g_ctx.intervalometerIntervalTime, true);
      }
      else {
        triggerCamerasPhase2();
      }
      return;
    }
  }
  g_ctx.camTimer.stop();
  g_ctx.camTriggerRunning = false;
  g_ctx.prevToggleCamVal = LOW;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// handleMirrorLockup() - If mirror lockup is enabled this sets shutter and focus pins for a brief time when first
//  entering photo mode
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void handleMirrorLockupPhase3() {
  g_ctx.camTriggerRunning = false;
  g_ctx.camTimer.stop();
}

void handleMirrorLockupPhase2() {
  for(uint8_t i=0; i<NUM_CAMERAS; ++i) {
    uint8_t mode = g_ctx.camSettings[i].getMode();
    uint8_t focusVal = ((mode == CA_MODE_PREFOCUS) || (mode == CA_MODE_SMART_PREFOCUS)) ? HIGH : LOW;
    uint8_t mirrorLockup = g_ctx.camSettings[i].getMirrorLockup();
  
    if (mirrorLockup && (mode != CA_MODE_NONE)) {
      hwPortPin focusPin = CAU::getCameraPin(i, FOCUS);
      hwPortPin shutterPin = CAU::getCameraPin(i, SHUTTER);
      CAU::digitalWrite(focusPin, focusVal);
      CAU::digitalWrite(shutterPin, LOW);
    }
  }
  uint64_t t = CATickTimer::convertTimeToTicks(0, 250000000);
  g_ctx.camTimer.start(handleMirrorLockupPhase3, t, true);
}

void handleMirrorLockup() {
  bool anyMirrorLockup = false;

  CA_ASSERT((g_ctx.camTriggerRunning == false), "Should never be running camera here");
  
  for(uint8_t i=0; i<NUM_CAMERAS; ++i) {
    uint8_t mode = g_ctx.camSettings[i].getMode();

    if (mode != CA_MODE_NONE) {
      uint8_t mirrorLockup = g_ctx.camSettings[i].getMirrorLockup();
  
      if (mirrorLockup) {
        hwPortPin focusPin = CAU::getCameraPin(i, FOCUS);
        hwPortPin shutterPin = CAU::getCameraPin(i, SHUTTER);
        CAU::digitalWrite(focusPin, HIGH);
        CAU::digitalWrite(shutterPin, HIGH);
        anyMirrorLockup = true;
      }
    }
  }

  if (anyMirrorLockup) {
    uint64_t t = CATickTimer::convertTimeToTicks(0, 250000000);
    g_ctx.camTimer.stop();
    g_ctx.camTimer.start(handleMirrorLockupPhase2, t, true);
    g_ctx.camTriggerRunning = true;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// startTriggerCameraState() - Set camera ports to correct levels when entering photo mode
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void startTriggerCameraState() {
  g_ctx.camTimer.stop();
  g_ctx.camTriggerRunning = false;
  g_ctx.prevToggleCamVal = LOW;
  
  for(uint8_t i=0; i<NUM_CAMERAS; ++i) {
    uint8_t mode = g_ctx.camSettings[i].getMode();
    if (mode != CA_MODE_NONE) {
      uint8_t focusVal = ((mode == CA_MODE_PREFOCUS) || (mode == CA_MODE_SMART_PREFOCUS)) ? HIGH : LOW;
      hwPortPin focusPin = CAU::getCameraPin(i, FOCUS);
      hwPortPin shutterPin = CAU::getCameraPin(i, SHUTTER);
      CAU::digitalWrite(focusPin, focusVal);
      CAU::digitalWrite(shutterPin, LOW);
    }
  }  
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// toggleCamerasInPhotoMode() - Toggle camera ports when in photo mode
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void toggleCamerasInPhotoMode() {
  if (camTriggerRunning() || g_ctx.state != CA_STATE_PHOTO_MODE) {
    return;
  }

  g_ctx.prevToggleCamVal = (g_ctx.prevToggleCamVal==LOW) ? HIGH : LOW;

  // Trigger flash while turning off camera (basically only in the double tap case
  if (g_ctx.prevToggleCamVal == LOW) {
    for (uint8_t i=0; i<NUM_CAMERAS; ++i) {
      CAPacketCamSettings &cs = g_ctx.camSettings[i];
      if (cs.getMode()==CA_MODE_FLASH) {
        hwPortPin shutterPin = CAU::getCameraPin(i, SHUTTER);
        CAU::digitalWrite(shutterPin, HIGH);
      }
    }
    delayMicroseconds(20);
    for (uint8_t i=0; i<NUM_CAMERAS; ++i) {
      CAPacketCamSettings &cs = g_ctx.camSettings[i];
      if (cs.getMode()==CA_MODE_FLASH) {
        hwPortPin shutterPin = CAU::getCameraPin(i, SHUTTER);
        CAU::digitalWrite(shutterPin, LOW);
      }
    }
  }

  // Trigger cameras
  for (uint8_t i=0; i<NUM_CAMERAS; ++i) {
    CAPacketCamSettings &cs = g_ctx.camSettings[i];
    if ((cs.getMode()==CA_MODE_CAMERA) || (cs.getMode()==CA_MODE_PREFOCUS) || (cs.getMode()==CA_MODE_SMART_PREFOCUS)) {
      uint8_t focusVal = g_ctx.prevToggleCamVal;
      if ((cs.getMode()==CA_MODE_PREFOCUS) || (cs.getMode()==CA_MODE_SMART_PREFOCUS)) {
        focusVal = HIGH;
      }
      
      hwPortPin focusPin = CAU::getCameraPin(i, FOCUS);
      hwPortPin shutterPin = CAU::getCameraPin(i, SHUTTER);
      CAU::digitalWrite(focusPin, focusVal);
      CAU::digitalWrite(shutterPin, g_ctx.prevToggleCamVal);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// resetCameraPorts() - Set camera ports to default state
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void resetCameraPorts() {
  g_ctx.camTimer.stop();
  g_ctx.camTriggerRunning = false;

  for(uint8_t i=0; i<NUM_CAMERAS; ++i) {
    hwPortPin focusPin = CAU::getCameraPin(i, FOCUS);
    hwPortPin shutterPin = CAU::getCameraPin(i, SHUTTER);
    CAU::digitalWrite(focusPin, LOW);
    CAU::digitalWrite(shutterPin, LOW);
  }  
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// resetModulePorts() - Set module ports to default state
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void resetModulePorts() {
  g_ctx.menuTimer.stop();
  
  for(uint8_t i=0; i<NUM_MODULES; ++i) {
    for(uint8_t j=0; j<6; ++j) {
      hwPortPin pp = CAU::getModulePin(i, j);
      CAU::pinMode(pp, OUTPUT);  // Set pins to high impedance
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// initCameraPins() - Setup camera pins
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void initCameraPins() {
  for(uint8_t i=0; i<NUM_CAMERAS; ++i) {
    hwPortPin focusPin = CAU::getCameraPin(i, FOCUS);
    hwPortPin shutterPin = CAU::getCameraPin(i, SHUTTER);
    CAU::pinMode(focusPin, OUTPUT);
    CAU::pinMode(shutterPin, OUTPUT);
  }
}

void setAllShutterPins() {
  for(uint8_t i=0; i<NUM_CAMERAS; ++i) {
    hwPortPin shutterPin = CAU::getCameraPin(i, SHUTTER);
    CAU::digitalWrite(shutterPin, HIGH);
  }
}

void setAllFocusPins() {
  for(uint8_t i=0; i<NUM_CAMERAS; ++i) {
    hwPortPin focusPin = CAU::getCameraPin(i, FOCUS);
    CAU::digitalWrite(focusPin, HIGH);
  }
}

void clearAllShutterPins() {
  for(uint8_t i=0; i<NUM_CAMERAS; ++i) {
    hwPortPin shutterPin = CAU::getCameraPin(i, SHUTTER);
    CAU::digitalWrite(shutterPin, LOW);
  }
}

void clearAllFocusPins() {
  for(uint8_t i=0; i<NUM_CAMERAS; ++i) {
    hwPortPin focusPin = CAU::getCameraPin(i, FOCUS);
    CAU::digitalWrite(focusPin, LOW);
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// blinkCameraPins() - Blink camera pins for debugging
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void blinkCameraPins() {
  for(uint8_t j=0; j<3; ++j) {
    setAllShutterPins();
    setAllFocusPins();
    delay(200);
    clearAllShutterPins();
    clearAllFocusPins();
    delay(200);
  }
  
  setAllShutterPins();
  setAllFocusPins();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// sortCamTiming() - Sorts the list of element from smallest time to largest time
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void sortCamTiming(CamElement0* arry, int8_t sz) {
  int8_t i = 1;

  // Insertion Sort
  while (i < sz) {
    CamElement0 x = arry[i];
    int8_t j = i-1;
    while ((j >= 0) && (arry[j].timeOffset > x.timeOffset)) {
      arry[j+1] = arry[j];
      --j;
    }
    arry[j+1] = x;
    ++i;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// sortCamTiming() - Converts timing from a relative to start of trigger to relative to previous element.
//  By making it relative to the previous element it reduces compute time to set system timers during the triggering.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void biasCamTiming(CamElement0* arry, int8_t sz) {
  // Loop through all camera events and find difference in offsets and store those rather than the absolute offset stored before this was called
  // Note this must be called after sorting
  uint64_t x0 = arry[0].timeOffset;
  uint64_t x1;
  for (int8_t i=1; i<sz; ++i) {
    x1 = arry[i].timeOffset - arry[i-1].timeOffset;
    if (x1 == 1) {
      x1 = 0; // Because of the integer divide by clock we get some off by 1 results.  Any interrupt will take way more than 1 clock so skip this
    }
    arry[i-1].timeOffset = x0;
    x0 = x1;
  }
  arry[sz-1].timeOffset = x0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// calcMasks() - Helper function that adds bits to the set/clear register masks based on current focus/shutter values
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void calcMasks(uint32_t &setMask, uint32_t &clearMask, uint8_t focusSig, uint8_t shutterSig, uint8_t cam) {
  hwPortPin focus = CAU::getCameraPin(cam, FOCUS);
  if (focusSig) {
    setMask |= (1<<focus.pin);
  }
  else {
    clearMask |= (1<<focus.pin);
  }
  
  hwPortPin shutter = CAU::getCameraPin(cam, SHUTTER);
  if (shutterSig) {
    setMask |= (1<<shutter.pin);
  }
  else {
    clearMask |= (1<<shutter.pin);
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// generateUnifiedElements() - This combines all the camElement0 elements that have exact same time into a CamElement1.
//  CamElement1 is just register set/clear masks and a time when to send them to send them to hw.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void generateUnifiedElements(CamElement0* camElements0, int8_t numCamElements0) {
  uint8_t i, j;
  CamElement1 *el = g_ctx.camElements;
  int8_t curEl = -1;

  memset(el, 0, sizeof(g_ctx.camElements));
  
  // Create a UnifiedElement with  each time generate an offsetTime, setMask (per port), clearMask (per port)
  // This is done by or-ing all the clear bits and all the set bits together
  // The basic idea here is we build masks for all the camera events happening at the same time and then set all those bits on the micro's ports in a single write instead of doing one bit at a time
  for(i=0; i<numCamElements0; ++i) {
    
    if ((i==0) || (camElements0[i].timeOffset!=0)) {
      // Increment the unified element in these cases because time has moved forward
      curEl++;
      el[curEl].timeOffset = camElements0[i].timeOffset;
    }
    calcMasks(el[curEl].setMask, el[curEl].clearMask, camElements0[i].focusSig, camElements0[i].shutterSig, camElements0[i].camOffset);
  }

  g_ctx.numCamElements = curEl+1;
  g_ctx.curCamElement = 0;

  //CA_LOG(CA_INFO, "Unified Camera List\n");
  //for(uint8_t i=0; i<g_ctx.numUnifiedCamTimerElements; ++i) {
  //  CA_LOG(CA_INFO, "%"PRId64" Set%#010x, Clear%#010x\n", g_ctx.unifiedCamTimerElements[i].timeOffset,
  //    g_ctx.unifiedCamTimerElements[i].setMask, g_ctx.unifiedCamTimerElements[i].clearMask,);
  //}

  memset(g_ctx.seqMask, 0, sizeof(g_ctx.seqMask));

  // Create a sequencer mask per sequencer bit (8 of them) which can be and-ed with the setMask/clearMask when trigger happens with the approriate sequencer value
  // The sequencer mask is basically any possible bit that could be set or cleared, but specifically excludes bits not allowed due to the sequencer settings from a camera port
  // This is done by walking through each each sequencer bit for each camera and or-ing its port bits into the sequencer mask for that sequencer bit
  for (i=0; i<NUM_SEQUENCER_BITS; ++i) {
    for(j=0; j<NUM_CAMERAS; ++j) {
      
      uint8_t seq = g_ctx.camSettings[j].getSequencer();
      
      // This check removes bits that shouldn't be in this sequencer mask
      if (seq & (1<<i)) {
        hwPortPin focus = CAU::getCameraPin(j, FOCUS);
        g_ctx.seqMask[i] |= (1<<focus.pin);

        hwPortPin shutter = CAU::getCameraPin(j, SHUTTER);
        g_ctx.seqMask[i] |= (1<<shutter.pin);
      }
    }
  }

  //CA_LOG(CA_INFO, "Unified Sequencer masks\n");
  //for(uint8_t i=0; i<NUM_SEQUENCER_BITS; ++i) {
  //  CA_LOG(CA_INFO, "%#010x\n", g_ctx.seqMask[i]);
  //}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// setupCamTiming() - This function is the main function that takes all camera timing values from the UI and converts
//  them into an efficent data structure that can be processed fast and efficently.  This is a multiphase process.
//  In the first phase we convert the UI data into an array of values that eliminates unneeded elements (cameras that are disabled)
//  and then it sorts all these elements.  The second phase we merge all the elements that occur at the exact same time
//  and cache off register masks to make it very quick to process these elements.
//  The whole point of this is that this can run slowly (outside the critical loop).  Then when we are triggering (in the critcal loop
//  we use these optimized data structures to run fast.
//  Requires a list of elements to be sorted and for time to be a relative offset from the previous element.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setupCamTiming(uint64_t extraCamDelayNs=0, uint64_t extraFlashDelayNs=0) {
  uint8_t i;
  uint8_t j = 0;
  uint64_t maxTotalTime = 0;
  uint8_t sequencerMask = 0;
  bool processCam[NUM_CAMERAS] = {false, false, false, false, false, false, false, false};
  CamElement0 camElements0[NUM_CAM_TIMER_ELEMENTS];
  uint8_t numCamElements0;

  // Phase 1 (converting camera data into CamElement0 elements and throwing out unneeded elements.
  for(i=0; i<NUM_CAMERAS; ++i) {
    uint8_t mode = g_ctx.camSettings[i].getMode();
    uint64_t extraDelayTimeNs = (mode == CA_MODE_FLASH) ? extraFlashDelayNs : extraCamDelayNs;
    uint64_t t0 = CATickTimer::convertTimeToTicks(g_ctx.camSettings[i].getDelaySeconds(), g_ctx.camSettings[i].getDelayNanoseconds(), extraDelayTimeNs);
    uint64_t t1 = CATickTimer::convertTimeToTicks(g_ctx.camSettings[i].getDurationSeconds(), g_ctx.camSettings[i].getDurationNanoseconds());
    uint64_t t2 = CATickTimer::convertTimeToTicks(g_ctx.camSettings[i].getPostDelaySeconds(), g_ctx.camSettings[i].getPostDelayNanoseconds());
    uint64_t t;
    uint8_t focusAfterDelay = (mode == CA_MODE_FLASH) ? LOW : HIGH;
    uint8_t focusAfterDuration = (mode == CA_MODE_PREFOCUS) ? HIGH : LOW;
    uint8_t sequencerVal = g_ctx.camSettings[i].getSequencer();

    t = t0+t1+t2;

    // Don't add to list if all times are zero OR the sequencerVal is 0 OR mode set to none
    //  in all these cases the camera port is disabled
    if ((t==0) || (sequencerVal==0) || (mode==CA_MODE_NONE)) {
      processCam[i] = false;
    }
    else {
      // Set focus/shutter pins right after delay time
      camElements0[j].timeOffset = t0;
      camElements0[j].camOffset = i;
      camElements0[j].focusSig = focusAfterDelay;
      camElements0[j].shutterSig = HIGH;
      camElements0[j++].sequencerVal = sequencerVal;

      // Set focus/shutter pins right after duration time
      camElements0[j].timeOffset = t0+t1;
      camElements0[j].camOffset = i;
      camElements0[j].focusSig = focusAfterDuration;
      camElements0[j].shutterSig = LOW;
      camElements0[j++].sequencerVal = sequencerVal;

      sequencerMask |= sequencerVal;
      maxTotalTime = max(t, maxTotalTime);
      processCam[i] = true;
    }
  }

  //for(i=0; i<NUM_CAM_TIMER_ELEMENTS; ++i) {
  //  CA_LOG(CA_INFO, "%"PRId64" %d %d %d %d\n", camElements0[i].timeOffset, camElements0[i].camOffset, camElements0[i].focusSig, camElements0[i].shutterSig, camElements0[i].sequencerVal);
  //}
  g_ctx.curSeqMask = sequencerMask;

  sortCamTiming(camElements0, j);
  // Don't need to sort the following because they are all inserted at the end with max time

  for(i=0; i<NUM_CAMERAS; ++i) {
    if (processCam[i]) {
      uint8_t mode = g_ctx.camSettings[i].getMode();
      uint8_t focusAtEnd = ((mode == CA_MODE_PREFOCUS) || (mode == CA_MODE_SMART_PREFOCUS)) ? HIGH : LOW;
      
      // Set focus/shutter pins at very end of trigger
      camElements0[j].timeOffset = maxTotalTime;
      camElements0[j].camOffset = i;
      camElements0[j].focusSig = focusAtEnd;
      camElements0[j].shutterSig = LOW;
      camElements0[j++].sequencerVal = g_ctx.camSettings[i].getSequencer();
    }
  }

  numCamElements0 = j;
  
  biasCamTiming(camElements0, numCamElements0);

  // Phase 2
  generateUnifiedElements(camElements0, numCamElements0);

  //CA_LOG(CA_INFO, "Unified Element List\n");
  //for(uint8_t i=0; i<g_ctx.numCamTimerElements; ++i) {
  //  CA_LOG(CA_INFO, "%d  %"PRId64"\n", g_ctx.camTimerElements[i].camOffset, g_ctx.camTimerElements[i].timeOffset);
  //}
}
#endif //TRIGGER_CAM_H

