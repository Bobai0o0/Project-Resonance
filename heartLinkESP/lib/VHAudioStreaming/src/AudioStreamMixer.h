#pragma once
#include <Interface.h>

class AudioStreamMixer : public IAudioStreamMixer
{
    IVHChannels * m_pVHChannels = nullptr;
    static AudioStreamMixer * _mPcmMixer;
    uint16_t mPcmSampleRate = DEFAULT_PCM_SAMPLE_RATE;
    IBoardTimer * mBoardTimer = nullptr;
    static constexpr int TIMER_FREQ = 40000000;
private:
    uint8_t AudioMixer(uint8_t pcmVal, uint8_t primVal,MixingStrategy mixStr);
    uint8_t interruptMix(uint8_t waveVal, uint8_t pcmVal);
    uint8_t largesValMix(uint8_t waveVal, uint8_t pcmVal);
    uint8_t additiveMix(uint8_t waveVal, uint8_t pcmVal);
    uint8_t customeMix(uint8_t waveVal, uint8_t pcmVal);
    void init() override;
    

protected:
   void timerLoop();

public:
    AudioStreamMixer() = default;
    AudioStreamMixer(uint16_t sampleRate);
    void setChannelObj(IVHChannels * pChnl);
    void startMixer() override;
    void stopMixer() override;
    static void pcmTimerFunc();
    
};
