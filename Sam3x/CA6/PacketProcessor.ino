void processIncomingPacket() {
  CAPacketHelper &ph = g_ctx.packetHelper;
  CAPacket &mUnpacker = ph.getUnpacker();
  uint8 *mData = ph.getData();

  if (ph.readOnePacket(mData)) {
    uint8 packetSize = mUnpacker.unpackSize();
    uint8 packetType = mUnpacker.unpackType();

    switch (packetType) {
      case PID_MENU_HEADER: {
        CAPacketMenuHeader unpack(mUnpacker);
        unpack.unpack();
        CAU::log("%d PID_MENU_HEADER - %d %d %s\n", packetSize, unpack.getMajorVersion(), unpack.getMinorVersion(), unpack.getMenuName());
        break;
      }
      case PID_NEW_ROW: {
        CAPacketNewRow unpack(mUnpacker);
        unpack.unpack();
        CAU::log("%d PID_NEW_ROW\n", packetSize);
        break;
      }
      case PID_NEW_CELL: {
        CAPacketNewCell unpack(mUnpacker);
        unpack.unpack();
        CAU::log("%d PID_NEW_CELL - %d %d\n", packetSize, unpack.getColumnPercentage(), unpack.getJustification());
        break;
      }
      case PID_COND_START: {
        CAPacketCondStart unpack(mUnpacker);
        unpack.unpack();
        CAU::log("%d PID_COND_START - %d %d %d\n", packetSize, unpack.getClientHostId(), unpack.getModAttribute(), unpack.getValue());
        break;
      }
      case PID_COND_END: {
        CAPacketCondEnd unpack(mUnpacker);
        unpack.unpack();
        CAU::log("%d PID_COND_END\n", packetSize);
        break;
      }
      case PID_TEXT_STATIC: {
        CAPacketTextStatic unpack(mUnpacker);
        unpack.unpack();
        CAU::log("%d PID_TEXT_STATIC - %s\n", packetSize, unpack.getText());
        break;
      }
      case PID_TEXT_DYNAMIC: {
        CAPacketTextDynamic unpack(mUnpacker);
        unpack.unpack();
        CAU::log("%d PID_TEXT_DYNAMIC - %d %s\n", packetSize, unpack.getClientHostId(), unpack.getText());
        break;
      }
      case PID_BUTTON: {
        CAPacketButton unpack(mUnpacker);
        unpack.unpack();
        CAU::log("%d PID_Button - %d %d %d %s\n", packetSize, unpack.getClientHostId(), unpack.getType(), unpack.getValue(), unpack.getText());
        break;
      }
      case PID_CHECK_BOX: {
        CAPacketCheckBox unpack(mUnpacker);
        unpack.unpack();
        CAU::log("%d PID_CHECK_BOX - %d %d\n", packetSize, unpack.getClientHostId(), unpack.getValue());
        break;
      }
      case PID_DROP_SELECT: {
        CAPacketDropSelect unpack(mUnpacker);
        unpack.unpack();
        CAU::log("%d PID_DROP_SELECT - %d %d %s\n", packetSize, unpack.getClientHostId(), unpack.getValue(), unpack.getText());
        break;
      }
      case PID_EDIT_NUMBER: {
        CAPacketEditNumber unpack(mUnpacker);
        unpack.unpack();
        CAU::log("%d PID_EDIT_NUMBER - %d %d %d %d %d %d\n", packetSize, unpack.getClientHostId(), unpack.getDigitsBeforeDecimal(),
                  unpack.getDigitsAfterDecimal(), unpack.getMinValue(), unpack.getMaxValue(), unpack.getValue());
        break;
      }
      case PID_TIME_BOX: {
        CAPacketTimeBox unpack(mUnpacker);
        unpack.unpack();
        CAU::log("%d PID_TIME_BOX - %d %d %d %d %d %d %d %d\n", packetSize, unpack.getClientHostId(), unpack.getEnableMask(),
                  unpack.getHours(), unpack.getMinutes(), unpack.getSeconds(), unpack.getMilliseconds(), unpack.getMicroseconds(),
                  unpack.getNanoseconds());
        break;
      }
      case PID_SCRIPT_END: {
        CAPacketScriptEnd unpack(mUnpacker);
        unpack.unpack();
        CAU::log("%d PID_SCRIPT_END\n", packetSize);
        break;
      }
      case PID_MENU_SELECT: {
        CAPacketMenuSelect unpack(mUnpacker);
        unpack.unpack();
        CAU::log("%d PID_MENU_SELECT - %d %d\n", packetSize, unpack.getMode(), unpack.getMenuNumber());

        g_ctx.procTable.funcInit[1](0);
        //ph.writePacketLogger("Message from sam3x");
        break;
      }
      case PID_LOGGER: {
        CAPacketLogger unpack(mUnpacker);
        unpack.unpack();
        CAU::log("%d PID_LOGGER - %s\n", packetSize, unpack.getLog());
        break;
      }
      case PID_CAM_STATE: {
        CAPacketCamState unpack(mUnpacker);
        unpack.unpack();
        CAU::log("%d PID_CAM_STATE - %d %d %d\n", packetSize, unpack.getMultiplier(), unpack.getFocus(), unpack.getShutter());
        break;
      }
      case PID_CAM_SETTINGS: {
        CAPacketCamSettings unpack(mUnpacker);
        unpack.unpack();
        CAU::log("%d PID_CAM_SETTINGS - %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n", packetSize, unpack.getCamPortNumber(),
                  unpack.getMode(), unpack.getDelayHours(), unpack.getDelayMinutes(), unpack.getDelaySeconds(),
                  unpack.getDelayMilliseconds(), unpack.getDelayMicroseconds(), unpack.getDurationHours(), unpack.getDurationMinutes(),
                  unpack.getDurationSeconds(), unpack.getDurationMilliseconds(), unpack.getDurationMicroseconds(), unpack.getSequencer(),
                  unpack.getApplyIntervalometer(), unpack.getSmartPreview(), unpack.getMirrorLockupEnable(),
                  unpack.getMirrorLockupMinutes(), unpack.getMirrorLockupSeconds(), unpack.getMirrorLockupMilliseconds());
        break;
      }
      case PID_INTERVALOMETER: {
        CAPacketIntervalometer unpack(mUnpacker);
        unpack.unpack();
        CAU::log("%d PID_INTERVALOMETER - %d %d %d %d %d %d %d %d %d %d %d\n", packetSize, unpack.getStartHours(), unpack.getStartMinutes(),
        unpack.getStartSeconds(), unpack.getStartMilliseconds(), unpack.getStartMicroseconds(), unpack.getIntervalHours(),
        unpack.getIntervalMinutes(), unpack.getIntervalSeconds(), unpack.getIntervalMilliseconds(), unpack.getIntervalMicroseconds(),
        unpack.getRepeats());
        break;
      }
      case PID_INTER_MODULE_LOGIC: {
        CAPacketInterModuleLogic unpack(mUnpacker);
        unpack.unpack();
        CAU::log("%d PID_INTER_MODULE_LOGIC - %d %d\n", packetSize, unpack.getLatchEnable(), unpack.getLogic());
        break;
      }
      case PID_CONTROL_FLAGS: {
        CAPacketControlFlags unpack(mUnpacker);
        unpack.unpack();
        CAU::log("%d PID_CONTROL_FLAGS - %d %d\n", packetSize, unpack.getSlaveModeEnable(), unpack.getExtraMessagesEnable());
        break;
      }
      break;
    }
    mUnpacker.resetBuffer();
  }
}
