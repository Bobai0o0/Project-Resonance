#pragma once
#include <datastructure.h>
#include <Utilities/VHUtilities.h>
#include <functional>

class IVHChannel;
class VHI2sChannel;
class VHChannels;

class IDevTools : public VHBaseType
{
public:
    IDevTools() : VHBaseType(IType::idev_tools) {}
    virtual void init() = 0;
    virtual int parseCommandList(std::string &strCmd) = 0;
};

class IEffectReceiver : public VHBaseType
{
protected:
    IDevTools *mDevTool;

public:
    using VHBaseType::init;
    IEffectReceiver() : VHBaseType(ieffect_receiver) { mDevTool = nullptr; };
    virtual void init() = 0;
    virtual int parseData(const unsigned char *data, unsigned short len) = 0;
    virtual void setDevToolPtr(IDevTools *pDevTool) { mDevTool = pDevTool; };
    virtual void setDataBuffSize(int val) = 0;
};
class II2SoundBus : public VHBaseType
{
public:
    II2SoundBus() : VHBaseType(IType::ii2s) {}
    virtual void init() = 0;
    virtual void begin(unsigned char bclk, unsigned char lrc, unsigned char din, int samplerate = 8000, int bitpersample = 16) = 0;
    virtual void stop() = 0;
    virtual void start() = 0;
    virtual void write(unsigned char val) = 0;
    virtual bool IsRunning() = 0;
};

class IVHChannels : public VHBaseType
{
private:
    /* data */
public:
    IVHChannels() : VHBaseType(IType::ichannel_list) {}
    virtual IVHChannel *getChannel(int chnlNum) = 0;
    virtual IVHChannel *getChnlByIndex(int idx) = 0;
    virtual void getChnlByTag(const char *tag, std::vector<unsigned char> &res) = 0;
    virtual void getAllChnls(std::vector<unsigned char> &res) = 0;
    virtual IVHChannel *getDuelChnl() = 0;
    virtual int getCount() = 0;
    virtual std::string toString() = 0;

    virtual void addPrimitive(Primitive prim, unsigned char chnlNo) = 0;
    virtual void addPrimitive(Primitive prim, const char *tags) = 0;
    virtual void addPrimitive(Primitive prim, std::vector<unsigned char> &chnl) = 0;

    virtual void addPrimitiveRenderQueue(Primitive prim, unsigned char chnlNo) = 0;
    virtual void addPrimitiveRenderQueue(Primitive prim, const char *tags) = 0;
    virtual void addPrimitiveRenderQueue(Primitive prim, std::vector<unsigned char> &chnl) = 0;

    virtual bool queueIsEmpty(unsigned char chnlNo) = 0;

    virtual void clearRenderQueue(unsigned char chnlNo) = 0;
    virtual void clearPrimitives() = 0;
    virtual void setPrimManagerPtr(class IPrimitiveManager *ptr) = 0;
    virtual void init() = 0;
    virtual void applyDSP(int channelNo, std::function<uint8_t(uint8_t)> dspCb) = 0;
    virtual VHPlaybackState playbackStatus(int channel) = 0;
    virtual void enableIdleHandler(uint8_t chnlNo, bool enable) = 0;
    virtual void changePwmFrequency(const unsigned char chnlNo, const unsigned int freqHz = PWM_FREQ) {};
};
class IAudioStreamMixer : public VHBaseType
{
protected:
    uint16_t mPcmSampleRate;

public:
    IAudioStreamMixer() : VHBaseType(IType::iaudio_streaming_mixer) {}
    using VHBaseType::init;
    virtual void setChannelObj(IVHChannels *pChnl) = 0;
    virtual void startMixer() = 0;
    virtual void stopMixer() = 0;
    void setSampleRate(const uint16_t rate) { mPcmSampleRate = rate; };
    virtual void init() = 0;
};

class IPrimitiveManager : public VHBaseType
{
private:
    /* data */
public:
    IPrimitiveManager() : VHBaseType(IType::iprimitive_man) {}
    virtual void init() = 0;
    virtual size_t getNumPrimitives() const = 0;
    virtual std::string getPrimName(size_t num) const = 0;
    virtual std::string toString() = 0;
};

const int PREMITIVE_COUNT = 8;
const std::string PREMITIVE_NAME[PREMITIVE_COUNT] =
	{"Pulse", "Tick", "Vibrate", "Pause", "Pcm Player", "Audio Stream", "Erm", "Sweep"};

class BoardProfile;
class IBoardQueue;
class IBoardMutex;
class IVHChannel
{
public:
    virtual void init() = 0;
    virtual void write(unsigned char val, bool IsPcm = false) = 0;
    virtual void writeMixer(unsigned char val) = 0;
    virtual void setBoardPtr(BoardProfile *ptr) = 0;
    virtual void setSampleRate(int sampleRate) = 0;
    virtual void setI2sSndBusPtr(II2SoundBus *ptr) = 0;
    virtual void setChnlMinMaxIntensity(unsigned char min, unsigned char max) = 0;
    virtual unsigned char getChannelNo() = 0;
    virtual void setPrimManagerPtr(IPrimitiveManager *ptr) = 0;
    virtual void getChnlTags(std::vector<std::string> &list) = 0;
    virtual bool IsDualChannel() = 0;
    virtual std::string toString() = 0;
    virtual bool IsActive(int option) = 0;
    virtual PrimitiveQueue *getQueue() = 0;
    virtual PrimitiveQueue *getRenderQueue() = 0;
    virtual void addPrimitive(Primitive &prim) = 0;
    virtual void clearRenderQueue() = 0;
    virtual void addPrimitiveRenderQueue(Primitive &prim) = 0;
    virtual bool queueIsEmpty() = 0;
    virtual void clearPrimitives() = 0;
    virtual std::string getSuppPremitivesAsString() = 0;
    virtual std::string getChnlTags() = 0;
    virtual MixingStrategy getMixingStrategy() const = 0;
    virtual void setMixingStrategy(MixingStrategy type) = 0;
    virtual unsigned char getPcmVal() const = 0;
    virtual unsigned char getPrimVal() = 0;
    virtual void setPrimVal(const unsigned char val) = 0;
    virtual void setPcmVal(const unsigned char val) = 0;
    virtual IBoardMutex *getMutexHandle() = 0;
    virtual void enableIdleHandler(bool enable) = 0;
    virtual bool isI2sChannel() = 0;
    virtual void applyDSP(std::function<uint8_t(uint8_t)> dspCb) = 0;
    virtual void attachCallback(CHANNEL_STATUS status, std::function<void(uint8_t)> statusCb) = 0;
    virtual void attachCallback(CHANNEL_STATUS status, std::function<void(uint8_t, uint8_t)> statusCb) = 0;
    virtual VHPlaybackState playbackStatus() const = 0;
    virtual void changePwmFrequency(const unsigned int freqHz) {};
};

class IAudioStreaming : public VHBaseType
{
private:
    /* data */
public:
    using VHBaseType::init;
    IAudioStreaming() : VHBaseType(IType::iaudio_streaming) {}
    virtual void init() = 0;
    virtual void setChannelPtr(IVHChannels *chnls) = 0;
    virtual void play(float intensity, std::vector<uint8_t> &chnls, float durationMs = -1.0f, uint8_t audioChannel = 0) = 0;
    virtual void premitiveDelay() = 0;
};