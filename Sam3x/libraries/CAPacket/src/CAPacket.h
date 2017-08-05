#ifndef __CAPACKET_H__
#define __CAPACKET_H__

#include <CAStd.h>

enum packetId  {PID_START_SENTINEL      =  0,  // Must be first
                PID_STRING              =  1,
                PID_UINT32              =  2,
                PID_TIME_BOX            =  3,
                PID_MENU_SELECT         =  4,
                PID_MENU_LIST           =  5,
                PID_MODULE_LIST         =  6,
                PID_CAM_STATE           =  7,
                PID_CAM_SETTINGS        =  8,
                PID_INTERVALOMETER      =  9,
                PID_CONTROL_FLAGS       = 10,
                PID_ECHO                = 11,
                PID_END_SENTINEL        = 12, // Must be last
               };

enum packetState { STATE_PACKER=1, STATE_UNPACKER=2 };

#define NULL_CLIENT_HOST_ID 0xff

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  CAPacket - This class handles converting from variables the app uses to a packed byte stream sent over the
//                  network.  This class manages the data buffer for either packing or unpacking modes.  Packing
//                  means converting from variables to the byte buffer.  Unpacking is converting from the byte
//                  buffer to variables.  There is one class per packet type and each of these classes has a 
//                  reference to this CAPacket class.
//
//             unpackSize() and unpackType() are separated from the main unpack, which is implemented in the 
//                  packet type specific classes, because having this the packet size and packet type early is
//                  useful in the code that uses these classes.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CAPacket
{
public:
    CAPacket(uint8_t state, uint8_t *buf, uint16_t bufSize);
    void resetBuffer();                                 // Resets the data buffer back to the start
    uint16_t unpackSize();                              // Unpacks the size of the packet
    uint8_t unpackType();                               // Unpacks the type of the packet
    uint32_t unpacker(uint8_t unpackBits);              // Unpacks 1..32 bits from the byte stream
    void unpackerString(String& str);                   // Unpacks a null terminated string from the byte stream
    void packer(uint32_t val, uint8_t packBits);        // Packs 1..32 bits into the byte stream
    void packerString(const char* src);                 // Packs a null terminated string into the byte stream
    void flushPacket();                                 // This just does some validation

private:    
    uint8_t mBitsUsed;        // Bits of an uncompleted byte (pack or unpack) used in the byte stream
    uint8_t mBitsVal;         // Value of the unused bits
    uint16_t mBytesUsed;      // Number of bytes used in the pack or unpack stream
    uint8_t mState;           // State of the class (prevents invalid use like unpacking from a pack buffer)
    uint8_t* mBuf;            // The byte stream
    uint16_t mBufSize;        // Size of the byte stream
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  CAPacketElement - This is an interface class for all the specific packet types that follow.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CAPacketElement {
public:
    virtual uint8_t getPacketType() = 0;
    virtual uint8_t getClientHostId() = 0;
    virtual void unpack() = 0;
    virtual uint16_t pack() = 0;
    virtual void packetToString(String& str) = 0;
    virtual void packetFromString(const String& str) = 0;
protected:
    CAPacket* mCAP;
    
    // This walks through the strings from javascript that contain all the data needed to generate a packet
    uint32_t getUint32FromString(uint16_t& startIndex, const String& str) {
        uint16_t val;
        uint16_t endIndex = str.indexOf('|', startIndex);
        CA_ASSERT(endIndex!=-1, "Failed check");
        val = (str.substring(startIndex, endIndex)).toInt();
        startIndex = endIndex+1;
        return val;
    }

    // This walks through the strings from javascript that contain all the data needed to generate a packet
    String getStringFromString(uint16_t& startIndex, const String& str) {
        String val;
        uint16_t endIndex = str.indexOf('|', startIndex);
        CA_ASSERT(endIndex!=-1, "Failed check");
        val = str.substring(startIndex, endIndex);
        startIndex = endIndex+1;
        return val;
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Below are many classes.  One for each packet type.
//   * All these classes have a constructor that attaches a reference of CAPacket to this class.
//   * All these classes have get*() methods to access private data members.
//   * All these classes have a set() method to set all the private data members.
//   * All these classes have an unpack() method to do the unpacking (not including packet size or type)
//   * All these classes have a pack() method to do the packing (including the packet size and type)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class CAPacketString : public CAPacketElement {
public:
    CAPacketString(CAPacket& caPacket);
    uint8_t getPacketType() {return PID_STRING;};
    uint8_t getClientHostId() {return mClientHostId;};
    uint8_t getFlags() {return mFlags;};
    const char* getString() {return mString.c_str();};
    void set(uint8_t clientHostId, uint8_t flags, String str);
    void unpack();
    uint16_t pack();
    void packetToString(String& str);
    void packetFromString(const String& str);
private:
    uint8_t mClientHostId;
    uint8_t mFlags;
    String mString;
};

class CAPacketUint32 : public CAPacketElement {
public:
    CAPacketUint32(CAPacket& caPacket);
    uint8_t getPacketType() {return PID_UINT32;};
    uint8_t getClientHostId() {return mClientHostId;};
    uint8_t getFlags() {return mFlags;};
    uint32_t getValue() {return mValue;};
    void set(uint8_t clientHostId, uint8_t flags, uint32_t value);
    void unpack();
    uint16_t pack();
    void packetToString(String& str);
    void packetFromString(const String& str);
private:
    uint8_t mClientHostId;
    uint8_t mFlags;
    uint32_t mValue;
};

class CAPacketTimeBox : public CAPacketElement {
public:
    CAPacketTimeBox(CAPacket& caPacket);
    uint8_t getPacketType() {return PID_TIME_BOX;};
    uint8_t getClientHostId() {return mClientHostId;};
    uint8_t getFlags() {return mFlags;};
    uint16_t getHours() {return mHours;};
    uint8_t getMinutes() {return mMinutes;};
    uint8_t getSeconds() {return mSeconds;};
    uint16_t getMilliseconds() {return mMilliseconds;};
    uint16_t getMicroseconds() {return mMicroseconds;};
    uint16_t getNanoseconds() {return mNanoseconds;};
    void set(uint8_t clientHostId, uint8_t flags, uint16_t hours, uint8_t minutes, uint8_t seconds,
                uint16_t milliseconds, uint16_t microseconds, uint16_t nanoseconds);
    void unpack();
    uint16_t pack();
    void packetToString(String& str);
    void packetFromString(const String& str);
private:
    uint8_t mClientHostId;
    uint8_t mFlags;
    uint16_t mHours;
    uint8_t mMinutes;
    uint8_t mSeconds;
    uint16_t mMilliseconds;
    uint16_t mMicroseconds;
    uint16_t mNanoseconds;
};

class CAPacketMenuSelect : public CAPacketElement {
public:
    CAPacketMenuSelect(CAPacket& caPacket);
    uint8_t getPacketType() {return PID_MENU_SELECT;};
    uint8_t getClientHostId() {return NULL_CLIENT_HOST_ID;};
    uint8_t getMode() {return mMode;};
    uint8_t getMenuNumber() {return mMenuNumber;};
    void set(uint8_t activate, uint8_t menuNumber);
    void unpack();
    uint16_t pack();
    void packetToString(String& str);
    void packetFromString(const String& str);
private:
    uint8_t mMode;
    uint8_t mMenuNumber;
};

class CAPacketMenuList : public CAPacketElement {
public:
    CAPacketMenuList(CAPacket& caPacket);
    uint8_t getPacketType() {return PID_MENU_LIST;};
    uint8_t getClientHostId() {return NULL_CLIENT_HOST_ID;};
    uint8_t getMenuId() {return mMenuId;};
    uint8_t getModuleId0() {return mModuleId0;};
    uint8_t getModuleMask0() {return mModuleMask0;};
    uint8_t getModuleId1() {return mModuleId1;};
    uint8_t getModuleMask1() {return mModuleMask1;};
    uint8_t getModuleId2() {return mModuleId2;};
    uint8_t getModuleMask2() {return mModuleMask2;};
    uint8_t getModuleId3() {return mModuleId3;};
    uint8_t getModuleMask3() {return mModuleMask3;};
    uint8_t getModuleTypeId0() {return mModuleTypeId0;};
    uint8_t getModuleTypeMask0() {return mModuleTypeMask0;};
    uint8_t getModuleTypeId1() {return mModuleTypeId1;};
    uint8_t getModuleTypeMask1() {return mModuleTypeMask1;};
    const char* getMenuName() {return mMenuName.c_str();};
    void set(uint8_t menuId, uint8_t moduleId0, uint8_t moduleMask0,  uint8_t moduleId1, uint8_t moduleMask1,
                uint8_t moduleId2, uint8_t moduleMask2, uint8_t moduleId3, uint8_t moduleMask3,
                uint8_t moduleTypeId0, uint8_t moduleTypeMask0, uint8_t moduleTypeId1, uint8_t moduleTypeMask1,
                String menuName);
    void unpack();
    uint16_t pack();
    void packetToString(String& str);
    void packetFromString(const String& str);
private:
    uint8_t mMenuId;
    uint8_t mModuleId0;
    uint8_t mModuleMask0;
    uint8_t mModuleId1;
    uint8_t mModuleMask1;
    uint8_t mModuleId2;
    uint8_t mModuleMask2;
    uint8_t mModuleId3;
    uint8_t mModuleMask3;
    uint8_t mModuleTypeId0;
    uint8_t mModuleTypeMask0;
    uint8_t mModuleTypeId1;
    uint8_t mModuleTypeMask1;
    String mMenuName;
};

class CAPacketModuleList : public CAPacketElement {
public:
    CAPacketModuleList(CAPacket& caPacket);
    uint8_t getPacketType() {return PID_MODULE_LIST;};
    uint8_t getClientHostId() {return NULL_CLIENT_HOST_ID;};
    uint8_t getModuleId() {return mModuleId;};
    uint8_t getModuleTypeId() {return mModuleTypeId;};
    const char* getModuleName() {return mModuleName.c_str();};
    void set(uint8_t moduleId, uint8_t moduleTypeId, String moduleName);
    void unpack();
    uint16_t pack();
    void packetToString(String& str);
    void packetFromString(const String& str);
private:
    uint8_t mModuleId;
    uint8_t mModuleTypeId;
    String mModuleName;
};

class CAPacketCamState : public CAPacketElement {
public:
    const uint8_t CAM0    = 0x01;
    const uint8_t CAM1    = 0x02;
    const uint8_t CAM2    = 0x04;
    const uint8_t CAM3    = 0x08;
    const uint8_t CAM4    = 0x10;
    const uint8_t CAM5    = 0x20;
    const uint8_t CAM6    = 0x40;
    const uint8_t CAM7    = 0x80;
    CAPacketCamState(CAPacket& caPacket);
    uint8_t getPacketType() {return PID_CAM_STATE;};
    uint8_t getClientHostId() {return NULL_CLIENT_HOST_ID;};
    uint8_t getMultiplier() {return mMultiplier;};
    uint8_t getFocus() {return mFocus;};
    uint8_t getShutter() {return mShutter;};
    void set(uint8_t multiplier, uint8_t focus, uint8_t shutter);
    void unpack();
    uint16_t pack();
    void packetToString(String& str);
    void packetFromString(const String& str);
private:
    uint8_t mMultiplier;
    uint8_t mFocus;
    uint8_t mShutter;
};

class CAPacketCamSettingsBase {
public:
    const uint8_t SEQ0    = 0x01;
    const uint8_t SEQ1    = 0x02;
    const uint8_t SEQ2    = 0x04;
    const uint8_t SEQ3    = 0x08;
    const uint8_t SEQ4    = 0x10;
    const uint8_t SEQ5    = 0x20;
    const uint8_t SEQ6    = 0x40;
    const uint8_t SEQ7    = 0x80;
    CAPacketCamSettingsBase();
    uint8_t getCamPortNumber() {return mCamPortNumber;};
    uint8_t getMode() {return mMode;};
    uint16_t getDelayHours() {return mDelayHours;};
    uint8_t getDelayMinutes() {return mDelayMinutes;};
    uint8_t getDelaySeconds() {return mDelaySeconds;};
    uint16_t getDelayMilliseconds() {return mDelayMilliseconds;};
    uint16_t getDelayMicroseconds() {return mDelayMicroseconds;};
    uint16_t getDurationHours() {return mDurationHours;};
    uint8_t getDurationMinutes() {return mDurationMinutes;};
    uint8_t getDurationSeconds() {return mDurationSeconds;};
    uint16_t getDurationMilliseconds() {return mDurationMilliseconds;};
    uint16_t getDurationMicroseconds() {return mDurationMicroseconds;};
    uint8_t getSequencer() {return mSequencer;};
    uint8_t getApplyIntervalometer() {return mApplyIntervalometer;};
    uint8_t getSmartPreview() {return mSmartPreview;};
    uint8_t getMirrorLockupEnable() {return mMirrorLockupEnable;};
    uint8_t getMirrorLockupMinutes() {return mMirrorLockupMinutes;};
    uint8_t getMirrorLockupSeconds() {return mMirrorLockupSeconds;};
    uint16_t getMirrorLockupMilliseconds() {return mMirrorLockupMilliseconds;};
    void set(uint8_t camPortNumber, uint8_t mode, uint16_t delayHours, uint8_t delayMinutes,
                uint8_t delaySeconds, uint16_t delayMilliseconds, uint16_t delayMicroseconds,
                uint16_t durationHours, uint8_t durationMinutes, uint8_t durationSeconds, 
                uint16_t durationMilliseconds, uint16_t durationMicroseconds, uint8_t sequencer,
                uint8_t applyIntervalometer, uint8_t smartPreview, uint8_t mirrorLockupEnable, 
                uint8_t mirrorLockupMinutes, uint8_t mirrorLockupSeconds, uint16_t mirrorLockupMilliseconds);
protected:
    uint8_t mCamPortNumber;
    uint8_t mMode;
    uint16_t mDelayHours;
    uint8_t mDelayMinutes;
    uint8_t mDelaySeconds;
    uint16_t mDelayMilliseconds;
    uint16_t mDelayMicroseconds;
    uint16_t mDurationHours;
    uint8_t mDurationMinutes;
    uint8_t mDurationSeconds;
    uint16_t mDurationMilliseconds;
    uint16_t mDurationMicroseconds;
    uint8_t mSequencer;
    uint8_t mApplyIntervalometer;
    uint8_t mSmartPreview;
    uint8_t mMirrorLockupEnable;
    uint8_t mMirrorLockupMinutes;
    uint8_t mMirrorLockupSeconds;
    uint16_t mMirrorLockupMilliseconds;
};

class CAPacketCamSettings : public CAPacketCamSettingsBase, public CAPacketElement  {
public:
    CAPacketCamSettings(CAPacket& caPacket);
    uint8_t getPacketType() {return PID_CAM_SETTINGS;};
    uint8_t getClientHostId() {return NULL_CLIENT_HOST_ID;};
    void unpack();
    uint16_t pack();
    void packetToString(String& str);
    void packetFromString(const String& str);
private:
};

class CAPacketIntervalometer : public CAPacketElement {
public:
    CAPacketIntervalometer(CAPacket& caPacket);
    uint8_t getPacketType() {return PID_INTERVALOMETER;};
    uint8_t getClientHostId() {return NULL_CLIENT_HOST_ID;};
    uint16_t getStartHours() {return mStartHours;};
    uint8_t getStartMinutes() {return mStartMinutes;};
    uint8_t getStartSeconds() {return mStartSeconds;};
    uint16_t getStartMilliseconds() {return mStartMilliseconds;};
    uint16_t getStartMicroseconds() {return mStartMicroseconds;};
    uint16_t getIntervalHours() {return mIntervalHours;};
    uint8_t getIntervalMinutes() {return mIntervalMinutes;};
    uint8_t getIntervalSeconds() {return mIntervalSeconds;};
    uint16_t getIntervalMilliseconds() {return mIntervalMilliseconds;};
    uint16_t getIntervalMicroseconds() {return mIntervalMicroseconds;};
    uint16_t getRepeats() {return mRepeats;};
    void set(uint16_t startHours, uint8_t startMinutes, uint8_t startSeconds, uint16_t startMilliseconds,
                uint16_t startMicroseconds, uint16_t intervalHours, uint8_t intervalMinutes,
                uint8_t intervalSeconds, uint16_t intervalMilliseconds, uint16_t intervalMicroseconds,
                uint16_t repeats);
    void unpack();
    uint16_t pack();
    void packetToString(String& str);
    void packetFromString(const String& str);
private:
    uint16_t mStartHours;
    uint8_t mStartMinutes;
    uint8_t mStartSeconds;
    uint16_t mStartMilliseconds;
    uint16_t mStartMicroseconds;
    uint16_t mIntervalHours;
    uint8_t mIntervalMinutes;
    uint8_t mIntervalSeconds;
    uint16_t mIntervalMilliseconds;
    uint16_t mIntervalMicroseconds;
    uint16_t mRepeats;
};

class CAPacketControlFlags : public CAPacketElement {
public:
    CAPacketControlFlags(CAPacket& caPacket);
    uint8_t getPacketType() {return PID_CONTROL_FLAGS;};
    uint8_t getClientHostId() {return NULL_CLIENT_HOST_ID;};
    uint8_t getSlaveModeEnable() {return mSlaveModeEnable;};
    uint8_t getExtraMessagesEnable() {return mExtraMessagesEnable;};
    void set(uint8_t slaveModeEnabe, uint8_t extraMessagesEnable);
    void unpack();
    uint16_t pack();
    void packetToString(String& str);
    void packetFromString(const String& str);
private:
    uint8_t mSlaveModeEnable;
    uint8_t mExtraMessagesEnable;
};

class CAPacketEcho : public CAPacketElement {
public:
    CAPacketEcho(CAPacket& caPacket);
    uint8_t getPacketType() {return PID_ECHO;};
    uint8_t getClientHostId() {return NULL_CLIENT_HOST_ID;};
    uint8_t getMode() {return mMode;};
    const char* getString() {return mString.c_str();};
    void set(uint8_t mode, String str);
    void unpack();
    uint16_t pack();
    void packetToString(String& str);
    void packetFromString(const String& str);
private:
    uint8_t mMode;
    String mString;
};

#endif // __CAPACKET_H__
