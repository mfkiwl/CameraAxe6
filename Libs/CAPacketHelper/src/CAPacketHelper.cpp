#include <CAPacketHelper.h>

uint16_t CAPacketHelper::serialFlowControlAvailable() {
    return mSerial->available();
}

void CAPacketHelper::serialFlowControlRead(uint8_t *buf, uint16_t length) {
    mSerial->readBytes(buf, length);
    // Code below prints out values of incoming packets for debug
    //for(int i = 0; i < length; ++i) {
    //    CAU::log("%d,",buf[i]);
    //}
    //CAU::log("\n");
}

void CAPacketHelper::serialFlowControlWrite(const uint8_t *buf, uint16_t length) {
    mSerial->write(buf, length);
}
    
void CAPacketHelper::init(HardwareSerial *serial) {
    mSerial = serial;
    flushGarbagePackets();
}

boolean CAPacketHelper::readOnePacket(uint8_t *data) {
    boolean ret = false;
    uint8_t avaliableBytes = serialFlowControlAvailable();
    
    // To read one packet you need to know the first two bytes in a packet is the size.  This code assumes that.
    // The third byte is always the packet type, but this code doesn't need to know that.
    
    if (avaliableBytes >= 2) {
        if (mSize == 0) {
            uint8_t buf[2];
            serialFlowControlRead(buf, 2);
            avaliableBytes -= 2;
            mSize = genPacketSize(buf[0], buf[1]);
            CA_ASSERT(mSize<MAX_PACKET_SIZE, "Invalid packet size");
        }

        if (avaliableBytes >= mSize-2) {
            data[0] = getPacketSize(mSize, 0);
            data[1] = getPacketSize(mSize, 1);
            serialFlowControlRead(data+2, mSize-2);
            mSize = 0;
            ret = true;
        }
    }
     return ret;
}

void CAPacketHelper::writeOnePacket(uint8_t *data) {
    uint16_t bufSize = genPacketSize(data[0], data[1]);

    if (bufSize >= MAX_PACKET_SIZE) {
        CA_ASSERT(0, "Exceeded Max packet size");
        return;
    }

    serialFlowControlWrite(data, bufSize);
}

void CAPacketHelper::writeMenu(const uint8_t *sData, uint16_t sz) {
    uint16_t currentPacketSize = 0;
    uint16_t currentPacketIndex = 0;
    for(uint16_t i=0; i<sz; ++i)
    {
        if (currentPacketSize == 0) {
            uint16_t b0 = pgm_read_byte_near(sData+(i++));
            uint16_t b1 = pgm_read_byte_near(sData+i);
            currentPacketSize = genPacketSize(b0, b1);
            mData[currentPacketIndex++] = b0;
            mData[currentPacketIndex++] = b1;
        }
        else {
            mData[currentPacketIndex++] = pgm_read_byte_near(sData+i);
            if (currentPacketIndex == currentPacketSize) {
                writeOnePacket(mData);
                mPacker.resetBuffer();
                currentPacketIndex = 0;
                currentPacketSize = 0;
            }
        }
    }
}

void CAPacketHelper::flushGarbagePackets() {
    while (mSerial->read() != -1){}   // flush out all the writes
 }

void CAPacketHelper::writePacketString(uint8_t clientHostId, uint8_t flags, const char* str) {
    CAPacketString pack0(mPacker);
    pack0.set(clientHostId, flags, str);
    pack0.pack();
    writeOnePacket(mData);
    mPacker.resetBuffer();
}

void CAPacketHelper::writePacketMenuList(uint8_t menuId, uint8_t moduleId0, uint8_t moduleMask0,  uint8_t moduleId1,
                uint8_t moduleMask1, uint8_t moduleId2, uint8_t moduleMask2, uint8_t moduleId3, uint8_t moduleMask3,
                uint8_t moduleTypeId0, uint8_t moduleTypeMask0, uint8_t moduleTypeId1, uint8_t moduleTypeMask1,
                String menuName) {
    CAPacketMenuList pack0(mPacker);
    pack0.set(menuId, moduleId0, moduleMask0,  moduleId1, moduleMask1, moduleId2, moduleMask2, moduleId3, moduleMask3,
                moduleTypeId0, moduleTypeMask0, moduleTypeId1, moduleTypeMask1, menuName);
    pack0.pack();
    writeOnePacket(mData);
    mPacker.resetBuffer();
}

void CAPacketHelper::writePacketEcho(uint8_t mode, const char* str) {
    CAPacketEcho pack0(mPacker);
    pack0.set(mode, str);
    pack0.pack();
    writeOnePacket(mData);
    mPacker.resetBuffer();
}