#pragma once

#include <vector>
#include <string>
#include <datastructure.h>
#include "BoardProfile.h"
#include "Interface.h"

class BoardProfile;
class IBoardQueue;
class IBoardMutex;

class VHChannel : public IVHChannel
{
    unsigned char mChnlNo;
    unsigned char mPin1;
    unsigned char mPin2;
    std::vector<std::string> mChnlDesc;
    // unsigned char mCtrlPin;
    BoardIdleCb mBoardIdleCb;
    bool isDualChannel;
    VHPlaybackState mPlaybackState = VHPlaybackState::VH_IDLE;
    unsigned char mMinIntensity, mMaxIntensity;
    int m_iPremitives;
    MixingStrategy mEnumMixingStat;
    PrimitiveQueue mQueue, mRenderQueue;
    unsigned char mPrimVal;
    unsigned int mPrimQueueEmptyCount;
    IBoardQueue *mQueueHandle, *mQueueIdleHandle, *mQueueHandleforPrim;
    IBoardMutex *mMutexHandle;
    virtual void chnlWrite(unsigned char val);
    static void idleHandlerTrd(void *param);
    void setZero();
    bool mIdleHandlerBool;
    BoardProfile *mBoardPtr;
    bool mIsI2sChannel;
    II2SoundBus *mI2sPtr;
    unsigned char mBclk, mLrc, mDin;
    IPrimitiveManager *mIPrimitiveManagerPtr = nullptr;
    std::function<uint8_t(uint8_t)> mDspCallback = nullptr;
    std::function<void(uint8_t)> mStatusCallbackSinglePin[CHANNEL_STATUS::MAX_CHANNEL_STATUS];
    std::function<void(uint8_t, uint8_t)> mStatusCallbackDualPin[CHANNEL_STATUS::MAX_CHANNEL_STATUS];

public:
    VHChannel(unsigned char chnl, std::vector<unsigned char> &pins, std::vector<std::string> list,
              BoardIdleCb cb = nullptr, MixingStrategy type = NO_MIX, int primOptions = -1);
    void init();
    ~VHChannel();
    void write(unsigned char val, bool IsPcm = false);
    void writeMixer(unsigned char val);
    void setBoardPtr(BoardProfile *ptr);
    void setSampleRate(int sampleRate);
    void setI2sSndBusPtr(II2SoundBus *ptr);
    void setChnlMinMaxIntensity(unsigned char min, unsigned char max);
    unsigned char getChannelNo();
    void setPrimManagerPtr(IPrimitiveManager *ptr);
    void getChnlTags(std::vector<std::string> &list);
    bool IsDualChannel();
    std::string toString();
    bool IsActive(int option);
    PrimitiveQueue *getQueue();
    PrimitiveQueue *getRenderQueue() override;
    void addPrimitive(Primitive &prim);
    void clearRenderQueue() override;
    void addPrimitiveRenderQueue(Primitive &prim) override;
    bool queueIsEmpty();
    void clearPrimitives();
    std::string getSuppPremitivesAsString();
    std::string getChnlTags();
    MixingStrategy getMixingStrategy() const;
    void setMixingStrategy(MixingStrategy type);
    unsigned char getPcmVal() const;
    unsigned char getPrimVal();
    void setPrimVal(const unsigned char val);
    void setPcmVal(const unsigned char val);
    IBoardMutex *getMutexHandle();
    void enableIdleHandler(bool enable);
    bool isI2sChannel();
    VHPlaybackState playbackStatus() const override { return mPlaybackState; }
    void applyDSP(std::function<uint8_t(uint8_t)> dspCb) override;
    void attachCallback(CHANNEL_STATUS status, std::function<void(uint8_t)> statusCb) override;
    void attachCallback(CHANNEL_STATUS status, std::function<void(uint8_t, uint8_t)> statusCb) override;
    void changePwmFrequency(const unsigned int freqHz) override;

private:
    void doCallback(CHANNEL_STATUS status);
};