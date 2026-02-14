#pragma once
#include <VectorHaptics.h>
// #if USE_VH_RING_BUFFER
#include "RingBuffer.h"
// #else
// #include "AudioTools.h"
// #endif
#include <Interface.h>
#include <cstdint>
#define DEFAULT_A2DP_SR 44100
#define DEFAULT_A2DP_CHNLS 2
#define BIT_A2DP_RATE 16

/// @cond HIDDEN_SYMBOL
class IBoardTimer;
class IVHChannels;
struct MixingInfo
{
    MixingStrategy MixType;
    uint8_t Channel;
};

enum BufferType
{
    UNKNOWN = 0,
    INT8,
    UINT8,
    INT16,
    UINT16,
    INT32,
    UINT32,
};

///@endcond

/**
 * @brief Audio streaming class to handle audio data streaming to the haptic device.
 * This class is used to stram audio data from external sources.
 *
 * @parblock
 * Please check the example : @ref AudioStreaming.ino "AudioStreaming"
 * @endparblock
 *
 */
class AudioStreaming : public IAudioStreaming
{

public:
    /**
     * @brief Construct a new Audio Streaming object
     *
     * @param sampleRate Sample rate of the incoming audio stream.Default is 44100Hz.
     * @param channelCount Channel count of the incoming audio stream.Default is 2 channels (stereo).
     * @param bitsPerSample Bits per sample of the incoming audio stream.Default is 16 bits.
     */
    AudioStreaming(uint32_t sampleRate = DEFAULT_A2DP_SR,
                   uint16_t channelCount = DEFAULT_A2DP_CHNLS,
                   uint8_t bitsPerSample = BIT_A2DP_RATE);

    BufferType mEnumBufferType = BufferType::UNKNOWN; /*!< Type of buffer */
// #if USE_VH_RING_BUFFER
    /**
     * @brief Initialize the int8_t type ring buffer to receive audio stream.
     *
     * @param buff an array of int8_t type to be used as ring buffer.
     */
    void init(VH::RingBuffer<int8_t> *buff);
    /**
     * @brief Initialize multiple int8_t type ring buffers (one per audio channel) to receive audio stream.
     *
     * @param buffs Array of ring buffers, one per audio channel (e.g., [leftBuffer, rightBuffer] for stereo).
     * @param numChannels Number of audio channels (must match mChannelCountInput).
     */
    void init(VH::RingBuffer<int8_t> **buffs, uint16_t numChannels);
    /**
     * @brief Initialize the uint8_t type ring buffer to receive audio stream.
     *
     * @param buff an array of uint8_t type to be used as ring buffer.
     */
    void init(VH::RingBuffer<uint8_t> *buff);
    /**
     * @brief Initialize multiple uint8_t type ring buffers (one per audio channel) to receive audio stream.
     *
     * @param buffs Array of ring buffers, one per audio channel (e.g., [leftBuffer, rightBuffer] for stereo).
     * @param numChannels Number of audio channels (must match mChannelCountInput).
     */
    void init(VH::RingBuffer<uint8_t> **buffs, uint16_t numChannels);
    /**
     * @brief Initialize the int16_t type ring buffer to receive audio stream.
     *
     * @param buff an array of int16_t type to be used as ring buffer.
     */
    void init(VH::RingBuffer<int16_t> *buff);
    /**
     * @brief Initialize multiple int16_t type ring buffers (one per audio channel) to receive audio stream.
     *
     * @param buffs Array of ring buffers, one per audio channel (e.g., [leftBuffer, rightBuffer] for stereo).
     * @param numChannels Number of audio channels (must match mChannelCountInput).
     */
    void init(VH::RingBuffer<int16_t> **buffs, uint16_t numChannels);
    /**
     * @brief Initialize the uint16_t type ring buffer to receive audio stream.
     *
     * @param buff an array of uint16_t type to be used as ring buffer.
     */
    void init(VH::RingBuffer<uint16_t> *buff);
    /**
     * @brief Initialize multiple uint16_t type ring buffers (one per audio channel) to receive audio stream.
     *
     * @param buffs Array of ring buffers, one per audio channel (e.g., [leftBuffer, rightBuffer] for stereo).
     * @param numChannels Number of audio channels (must match mChannelCountInput).
     */
    void init(VH::RingBuffer<uint16_t> **buffs, uint16_t numChannels);
    /**
     * @brief Initialize the int32_t type ring buffer to receive audio stream.
     *
     * @param buff an array of int32_t type to be used as ring buffer.
     */
    void init(VH::RingBuffer<int32_t> *buff);
    /**
     * @brief Initialize multiple int32_t type ring buffers (one per audio channel) to receive audio stream.
     *
     * @param buffs Array of ring buffers, one per audio channel (e.g., [leftBuffer, rightBuffer] for stereo).
     * @param numChannels Number of audio channels (must match mChannelCountInput).
     */
    void init(VH::RingBuffer<int32_t> **buffs, uint16_t numChannels);
    /**
     * @brief Initialize the uint32_t type ring buffer to receive audio stream.
     *
     * @param buff an array of uint32_t type to be used as ring buffer.
     */
    void init(VH::RingBuffer<uint32_t> *buff);
    /**
     * @brief Initialize multiple uint32_t type ring buffers (one per audio channel) to receive audio stream.
     *
     * @param buffs Array of ring buffers, one per audio channel (e.g., [leftBuffer, rightBuffer] for stereo).
     * @param numChannels Number of audio channels (must match mChannelCountInput).
     */
    void init(VH::RingBuffer<uint32_t> **buffs, uint16_t numChannels);
// #else
//     void init(RingBuffer<int8_t> *buff);
//     void init(RingBuffer<int8_t> **buffs, uint16_t numChannels);
//     void init(RingBuffer<uint8_t> *buff);
//     void init(RingBuffer<uint8_t> **buffs, uint16_t numChannels);
//     void init(RingBuffer<int16_t> *buff);
//     void init(RingBuffer<int16_t> **buffs, uint16_t numChannels);
//     void init(RingBuffer<uint16_t> *buff);
//     void init(RingBuffer<uint16_t> **buffs, uint16_t numChannels);
//     void init(RingBuffer<int32_t> *buff);
//     void init(RingBuffer<int32_t> **buffs, uint16_t numChannels);
//     void init(RingBuffer<uint32_t> *buff);
//     void init(RingBuffer<uint32_t> **buffs, uint16_t numChannels);
// #endif

    /**
     * @brief initialize the audio streaming object.
     *
     */
    void init() override;
    /**
     * @brief Write audio samples to the ring buffer.
     *
     * @param samples Pointer to the array of audio samples.
     * @param sampleCount Number of samples to write.
     */
    void write(const int16_t *samples, size_t sampleCount);
    /**
     * @brief Write audio samples to the ring buffer.
     *
     * @param samples Pointer to the array of audio samples.
     * @param sampleCount Number of samples to write.
     */
    void write(const uint8_t *samples, size_t sampleCount);
    /**
     * @brief Set a custom audio DSP function to process audio samples before streaming.
     *
     * @param dspCallback A function that takes a uint8_t sample and returns a processed uint8_t sample.
     */
    void setAudioDSP(std::function<uint8_t(uint8_t)> dspCallback);
    /**
     * @brief Enable mixing for a specific channel with the given mixing strategy.
     *
     * @param channel The channel number to enable mixing on.
     * @param mixType The mixing strategy to use.
     *
     * Please check the Example @ref Mixing_Primitives_with_Streamed_haptics.ino "Mixing_Primitives_with_Streamed_haptics"
     */
    void enableMixing(uint8_t channel, MixingStrategy mixType);
    /**
     * @brief Get the Instance AudioStreaming object
     *
     * @return AudioStreaming*
     */
    AudioStreaming *getInstance();
    /**
     * @brief Play or stop audio streaming on specified channels with intensity control.
     *
     * @param intensity Intensity of the audio stream (0.0 to 1.0). Use 0.0 to stop, > 0.0 to start/play.
     * @param chnls A vector of channel numbers to play the audio stream on.
     * @param durationMs Duration in ms to keep the stream on (use VH_AUDIO_STREAM_INFINITY or negative for infinite).
     * @param audioChannel 0 = mix all channels (default), 1 = first channel, 2 = second, ... N = Nth (up to stream channel count).
     */
    void play(float intensity, std::vector<uint8_t> &chnls, float durationMs = -1.0f, uint8_t audioChannel = 0) override;
    /**
     * @brief Set the maximum output sample rate for audio streaming.
     * This function sets the maximum output sample rate for audio streaming.
     * For example if you set maximum sample rate as 8000Hz and input audio stream is 44100Hz,
     * the out put sample rate is 44100/6=7350Hz. where 6 is the sampling factor.
     * The input audio stream will be resampled to this rate if necessary.
     *
     * @param sr The desired maximum output sample rate in Hz. Default is 16000 Hz.
     */
    void setMaxOutSampleRate(uint32_t sr);
    /**
     * @brief Set the channel object to be used for audio streaming.
     *
     * @param chnls Pointer to the IVHChannels object.
     */
    void setChannelPtr(IVHChannels *chnls) override;

private:
    void processFinalPCM(const uint8_t *buff, const size_t size);
    void premitiveDelay() override;
    int getFactor(uint32_t sampleRate);
    void initMixing();
    static AudioStreaming *_mAudioStreaming;
    bool mPlay = false;
    void *mBuffer = nullptr;
    void play();
    void stop();
    void audioTimerCb();
    uint32_t mSampleRateInput;
    uint16_t mChannelCountInput;
    uint8_t mBitsPerSampleInput;
    static constexpr int TIMER_FREQ = 40000000;
    uint32_t mSampleRateOut = DEFAULT_PCM_SAMPLE_RATE;
    IBoardTimer *mBoardTimer = nullptr;
    IVHChannels *mChannels = nullptr;
    std::vector<uint8_t> mChnls;
    std::vector<unsigned long> mChnlEndTimes; /* 0 = infinite, else millis() at which to stop */
    std::vector<uint8_t> mChnlIntensities; /* Intensity per channel (0-255, where 255 = 1.0) - using uint8_t for ISR safety */
    std::vector<uint8_t> mChnlAudioChannels; /* Audio channel mapping per output channel (which audio channel to play from) */
    std::vector<MixingInfo> mMixingInfo;
    std::unique_ptr<IAudioStreamMixer> mAudioStreamMixer = nullptr;
    std::function<uint8_t(uint8_t)> mAudioDspCallback = nullptr;
    bool pop(uint8_t &val, uint8_t audioChannel = 0);
    bool push(const uint8_t &val, uint8_t audioChannel = 0);
    void *mBuffers = nullptr; /* Array of buffers (one per audio channel) when multi-channel mode is enabled */
    bool mMultiChannelMode = false; /* Whether multiple buffers are being used (one per audio channel) */

    // Static buffers to eliminate heap allocations
    static constexpr size_t MAX_DECIMATED_BUFFER_SIZE = VH_MAX_AUDIOSTREAM_BUFF * 4;  
    static constexpr size_t MAX_MONO_BUFFER_SIZE = VH_MAX_AUDIOSTREAM_BUFF;           
    static constexpr size_t MAX_OUTPUT_BUFFER_SIZE = VH_MAX_AUDIOSTREAM_BUFF;         
    
    uint8_t m_decimated_buffer[MAX_DECIMATED_BUFFER_SIZE];
    int16_t m_mono16_buffer[MAX_MONO_BUFFER_SIZE];
    uint8_t m_output8_buffer[MAX_OUTPUT_BUFFER_SIZE];
};

using VHAudioStreaming = AudioStreaming;

/**
 * @example AudioStreaming.ino
 *
 */