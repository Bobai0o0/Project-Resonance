#pragma once
//#define USE_VH_RING_BUFFER 1
#include "Utilities/VHUtilities.h"
#include <vector>
#include <VHEffects.h>
#include <datastructure.h>
#include <memory>
#include <initializer_list>
#include <VHChannels/VHChannels.h>
#include <VHPrimitives/VHPrimitives.h>

typedef void (*WriteToPinCB)(unsigned char val);
typedef unsigned long (*MicrosCB)();
typedef void (*DelayCB)(unsigned int ms);

/*! @addtogroup Primitive Types
 *  Documentation for Macros
 *  @{
 */

/**
 * @brief Helper macro to create a VHPulse effect.
 *
 * This macro wraps the construction of a `VHPulse` object into a
 * `std::unique_ptr<IVhEffect>`. It forwards the provided arguments to
 * the `VHPulse` constructor.
 *
 * Example usage:
 * @code
 * vh.play(PULSE(1.0, 10, 0.8));
 * @endcode
 *
 * @param  duration Duration of the pulse effect in milliseconds.Default is 6.25ms. Range is 1ms to 200ms
 * @param  intensity Intensity of the pulse effect.Default is 0.70. Range is 0.0 to 1.0
 * @param  sharpness Sharpness of the pulse effect.Default is 0.5. Range is 0.0 to 1.0
 * @return std::unique_ptr<IVhEffect> Owning pointer to the created effect.
 */
#define PULSE(...) std::unique_ptr<IVhEffect>(new VHPulse(__VA_ARGS__))
/**
 * @brief Helper macro to create a VHTick effect.
 *
 * This macro wraps the construction of a `VHTick` object into a
 * `std::unique_ptr<IVhEffect>`. It forwards the provided arguments to
 * the `VHTick` constructor.
 *
 * Example usage:
 * @code
 * vh.play(TICK(1,10,0.5));
 * @endcode
 *
 * @param  duration Duration of the tick effect in milliseconds.Default is 6.25ms. Range is 1ms to 200ms
 * @param  intensity Intensity of the tick effect.Default is 0.70. Range is 0.0 to 1.0
 * @param  sharpness Sharpness of the tick effect.Default is 1.0. Range is 0.0 to 1.0
 * @return std::unique_ptr<IVhEffect> Owning pointer to the created effect.
 */
#define TICK(...) std::unique_ptr<IVhEffect>(new VHTick(__VA_ARGS__))
/**
 * @brief Helper macro to create a VHPause effect.
 *
 * This macro wraps the construction of a `VHPause` object into a
 * `std::unique_ptr<IVhEffect>`. It forwards the provided arguments to
 * the `VHPause` constructor.
 *
 * Example usage:
 * @code
 * vh.play(PAUSE(100));
 * @endcode
 *
 * @param  duration Duration of the tick effect in milliseconds.Default is 500ms. Range is 1ms to 10000ms
 * @return std::unique_ptr<IVhEffect> Owning pointer to the created effect.
 */
#define PAUSE(...) std::unique_ptr<IVhEffect>(new VHPause(__VA_ARGS__))
/**
 * @brief Helper macro to create a VHVibrate effect.
 * This macro wraps the construction of a `VHVibrate` object into a
 * `std::unique_ptr<IVhEffect>`. It forwards the provided arguments to
 * the `VHVibrate` constructor.
 *
 * Example usage:
 * @code
 * vh.play(VIBRATE(1000, 1, 100, 1));
 * @endcode
 * @param  duration Duration of the vibrate effect in milliseconds.Default duration is 1000/resonant frequency. Range is 1ms to 200ms
 * @param  intensity Intensity of the vibrate effect.Default is 0.70. Range is 0.0 to 1.0
 * @param  frequency Frequency of the vibrate effect in Hz.Default is resonant frequency of the motor. Range is 0Hz to 1000Hz
 * @param  sharpness Sharpness of the vibrate effect.Default is 0.0. Range is 0.0 to 1.0
 * @return std::unique_ptr<IVhEffect> Owning pointer to the created effect.
 *
 */
#define VIBRATE(...) std::unique_ptr<IVhEffect>(new VHVibrate(__VA_ARGS__))
/**
 * @brief Playes the PCM data as a haptic effect.
 *
 * Example usage:
 * @code
 * const uint8_t pcmData[] = { PCM data bytes };
 * vh.play(PCM(pcmData, sizeof(pcmData)));
 * @endcode
 *
 * @param  pcmData An 8-bit unsigned integer array containing the PCM data.
 * @param  size size of the pcm data array.
 *
 */
#define PCM(...) std::unique_ptr<IVhEffect>(new VH_Pcm(__VA_ARGS__))
/**
 * @brief Plays or stops audio streaming with intensity control.
 *
 * Example usage:
 * @code
 * // One argument: duration only (intensity 1.0, mix all channels)
 * vh.play(AUDIO_STREAM(VH_AUDIO_STREAM_INFINITY));
 *
 * // Two arguments: duration and intensity
 * vh.play(AUDIO_STREAM(5000, 0.5));
 *
 * // To stop audio streaming (intensity = 0)
 * vh.play(AUDIO_STREAM(VH_AUDIO_STREAM_INFINITY, 0.0));
 *
 * // Three arguments: duration, intensity, audio channel (0=mix, 1=first, 2=second, ... up to N)
 * vh.play(AUDIO_STREAM(VH_AUDIO_STREAM_INFINITY, 1.0, 1));  // first channel (e.g. left)
 * vh.play(AUDIO_STREAM(VH_AUDIO_STREAM_INFINITY, 1.0, 2));  // second channel (e.g. right)
 * @endcode
 *
 * @param  duration Duration in milliseconds, or VH_AUDIO_STREAM_INFINITY to keep on until stopped.
 * @param  intensity Intensity (0.0 to 1.0). Default 1.0 when omitted. Use 0.0 to stop.
 * @param  audioChannel 0 = mix all channels (default), 1 = first channel, 2 = second, ... N = Nth (up to stream channel count).
 */
/** Duration value meaning "keep audio stream on until explicitly stopped". Do not use symbol \c infinity (conflicts with math.h). */
constexpr float VH_AUDIO_STREAM_INFINITY = -1.0f;

// AUDIO_STREAM macro forwards arguments to VHAudio. Supports:
//   AUDIO_STREAM(duration)           -- intensity 1.0, channel 0 (mix)
//   AUDIO_STREAM(duration, intensity)
//   AUDIO_STREAM(duration, intensity, audioChannel)  -- 0=mix, 1..N=channel index
#define AUDIO_STREAM(...) std::unique_ptr<IVhEffect>(new VHAudio(__VA_ARGS__))
/**
 * @brief Helper macro to create a VHErm effect.
 *
 * This macro wraps the construction of a `VHErm` object into a
 * `std::unique_ptr<IVhEffect>`. It forwards the provided arguments to
 * the `VHErm` constructor.
 *
 * Example usage:
 * @code
 * vh.play(ERM(1000, 1.0));
 * @endcode
 *
 * @param  intensity Intensity of the tick effect.Default is 1.0 Range is -0.1 to 1.0
 * @param  duration Duration of the tick effect in milliseconds.Default is 1000ms. Range is 1ms to 10000ms
 * @return std::unique_ptr<IVhEffect> Owning pointer to the created effect.
 */
#define ERM(...) std::unique_ptr<IVhEffect>(new VHErm(__VA_ARGS__))
/**
 * @brief Helper macro to create a VHSweep effect (smooth transition of intensity, frequency, sharpness).
 *
 * Example usage:
 * @code
 * vh.play(SWEEP(1000, 1, 100, 0.5, 1, 500, 0.5, EaseInCubic, EaseInCubic, EaseInCubic));
 * @endcode
 *
 * @param duration Duration in ms.
 * @param startIntensity Start intensity (0-1). Use -1 for default.
 * @param startFrequency Start frequency in Hz.
 * @param startSharpness Start sharpness (0-1).
 * @param endIntensity End intensity (0-1).
 * @param endFrequency End frequency in Hz.
 * @param endSharpness End sharpness (0-1).
 * @param transIntensity Transition type for intensity (e.g. Linear, EaseInCubic).
 * @param transFrequency Transition type for frequency.
 * @param transSharpness Transition type for sharpness.
 */
#define SWEEP(...) std::unique_ptr<IVhEffect>(new VHSweep(__VA_ARGS__))
/*! @} */

#define ENABLE_MIXING true
#define DISABLE_MIXING false
#define VH_CORE_CHANNELS
#define VH_EFFECT_RECEIVER
#define VH_PCM_PLAYER
#define VH_PCM_MIXER
#define VH_BT_CLASSIC
#define VH_BT_LOW_ENERGY
#define VH_WIFI
#define VH_CORE
#define VH_DEV_TOOLS
#define VH_DEPRECATED_API 1
class VectorHaptics;
class IVHChannel;
class BoardProfile;
class BoardBasic;
class IVHChannels;
class IEffectReceiver;
class IDevTools;
class IPcmStream;
class IPcmMixer;
class II2SoundBus;
class IBoardI2sMan;
class ICustomPrim;
class IPrimitiveManager;
class IVhEffect;
class IAudioStreaming;

class VHCoreBasic
{
private:
    const float SLICE = HALF_PI / LUT_SIZE;
    void implementation2(unsigned char intensity, float duration, float sharpness);
    void write(unsigned char val);
    unsigned char floatToUint8t(float val);

protected:
    VectorHaptics *mVhPtr;
    BoardProfile *m_pBoard;
    BoardBasic *m_pBoardBasic;

public:
    VHCoreBasic(VectorHaptics *vh, BoardProfile *pBoard);
    VHCoreBasic(VectorHaptics *vh, BoardBasic *pBoard);
    ~VHCoreBasic();
    virtual void init() {};
    virtual void vibrate(unsigned long duration, float intensity, float frequency, float sharpness);
    virtual void sweep(unsigned long duration, StartEndParam &intensity, StartEndParam &frequency, StartEndParam &sharpness);
    virtual void pulse(unsigned long duration, float intensity, float sharpness);
    virtual void tick(unsigned long duration, float intensity, float sharpness);
    virtual void erm(float intensity, unsigned long duration);
    virtual void pause(unsigned long duration);
    float getApproxSineValFromLookUpTable(float x);
    virtual bool effectReceiver(IVHChannel *chnl) { return false; };
    virtual int renderQueue(IVHChannel *chnl) {return false;};
    void delayPrim(unsigned int msec);
    unsigned long getMicros();
};

/**
 * @internal
 * This class is for internal use only.
 */
class VHCoreMultiChnl : public VHCoreBasic
{
private:
    unsigned char getPrimitiveValue(class Primitive *prim);
    class IBoardQueue *m_pQueueHandle;
    void sendEffResToTerminal(Primitive *prim);

public:
    VHCoreMultiChnl(VectorHaptics *vh, BoardProfile *pBoard);
    ~VHCoreMultiChnl();
    virtual void init() override;
    bool effectReceiver(IVHChannel *chnl) override;
    int renderQueue(IVHChannel *chnl) override;
    static void resMsgThread(void *param);
};

/**
 * @mainpage Vector Haptics Library
 *
 * <h3>Below are available Libraries:</h3>
 *
 * 1. @ref VHChannels "VHChannels"
 * 2. @ref VHDevTools "VHDevTools"
 * 3. @ref VHEffectReceiver "VHEffectReceiver"
 * 4. @ref VHBLEApiHelper "VHBLELibrary"
 * 5. @ref AudioStreaming "VHAudioStreaming"
 * 6. @ref VHChannelsI2s "VHChannelsI2s"
 * 7. @ref VectorHaptics "VectorHaptics"
 * 8. @ref BoardProfile "VHBoardProfiles"
 *
 * <h3>Olimex ESP32 Configuration : </h3><br>
 * To configure the build system for **Olimex ESP32**, follow these steps:
 *
 * 1. In the root `CMakeLists.txt` file:
 *    - Uncomment **line 5** and **line 12** to enable necessary configurations for Olimex boards.
 *
 * 2. In the `CMakeLists.txt` where ADF board support is needed:
 *    - Locate the line:
 *      @code
 *      option(USE_ADF "Enable ADF support" OFF)
 *      @endcode
 *    - Change it to:
 *      @code
 *      option(USE_ADF "Enable ADF support" ON)
 *      @endcode
 *
 * These settings ensure proper integration and support for Olimex ESP32-based audio and BLE applications.
 *
 * @parblock
 * Please check the Example : @ref ADF_example.ino "ADF_example"
 * @endparblock
 *
 * <h3>Platform IO Configuration : </h3><br>
 *  Install Platform IO and you can directly burn it to the board. platform.ini file is already included in this package
 *
 * <h3>Arduino IDE Configuration : </h3><br>
 *  @li Copy the lib folder into Documents/Arduino and rename it to libraries.
 *  @li Go to File → Preferences → Additional Board Manager URLs and add:
 *  @code https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json @endcode
 *  @li Go to Tools → Board → Board Manager, search for esp32.
 *  @li Install esp32 by Espressif Systems, version 3.3.0.
 *  @li After installation, go to File → Examples → Vectorhaptics, open the required example, and flash it to your board.
 *
 *
 * <h3>Bluetooth Audio (A2DP) support information:</h3><br>
 *  @li Bluetooth Audio (A2DP) is supported **only** on the classic ESP32 MCU.
 *  @li For **ESP32-S3** and **ESP32-C3**, only BLE is supported (no A2DP).
 *  @li **ESP32-S2** has no Bluetooth support at all.
 * @parblock To enable A2DP, edit `src/CMakeLists.txt` and update:
 * @code
 * option(USE_A2DP "Enable A2DP support" OFF)
 * @endcode
 * to
 * @code
 * option(USE_A2DP "Enable A2DP support" ON)
 * @endcode
 * @endparblock
 *
 */

/**
 * @brief Main API class for interacting with the VectorHaptics library.
 * @b Example:
 * @code {.cpp}
 *  VectorHaptics vh;
 *  vh.loadBoard("ESP32");
 * @endcode
 */

class VectorHaptics
{
    IVHChannels *mChannels;
    VHCoreBasic *mCore;
    IEffectReceiver *mEff;
    IDevTools *mDev;
    IPcmStream *mPcmStream;
    IPcmMixer *mPcmMixer;
    II2SoundBus *mII2SoundBus;
    static VectorHaptics *_mVectorHaptics;
    int m_iErrorLogger;
    BoardProfile *mBoardPtr;
    IBoardI2sMan *mI2sBoardMan;
    IBoardMutex *mMutex;
    IPrimitiveManager *mIPrimitiveManager;
    DeviceInfo mDeviceInfo;
    std::string mErrorString, mMessage;
    bool mDelayRun = true;
    // IBoardQueue *mErrorHand, *mMsgHnd;
    IBoardQueue *mMsgHnd;
    WriteToPinCB mWriteToPinCB;
    MicrosCB mMicrosCB;
    DelayCB mDelayCB;
    PcmchunkCb mPcmChunkCB;
    IAudioStreaming *mAudioStreaming = nullptr;
    bool mBasicBoard = false;

public:
    /**
     * @brief Construct a new Vector Haptics object
     * @code {.cpp}
     * VectorHaptics vh;
     * @endcode
     *
     *
     */
    VectorHaptics();
    /**
     * @brief Load the Specified Board Profile
     *
     * @param profile  The profile name of the board to load (e.g., "ESP32", "STM32", etc.)
     * @return BoardProfile*
     */
    BoardProfile *loadBoard(std::string profile);
    /**
     * @brief Load the specified board profile.
     *
     * This function loads the given board profile.
     *
     * @param pBoard Pointer to the board profile to load.
     * @return BoardProfile* Pointer to the loaded board profile.
     *
     * @code
     *     BoardProfile board;
     *     vh.loadBoard(&board);
     * @endcode
     */
    BoardProfile *loadBoard(BoardProfile *pBoard);

    /**
     * @brief Initialize the VectorHaptics library with a basic board interface. Remember that this is a single channel configuration
     *
     * @param pBoard the pointer returned from loadBoard function
     */
    void init(BoardBasic *pBoard);
    /**
     * @brief Initialize the VectorHaptics library with a full board profile and a list of library objects to be used.
     *
     * This function sets up the VectorHaptics library by initializing it with the provided board profile and a collection of library objects.
     *
     * @param pBoard the pointer returned from loadBoard function
     * @param LibraryObjeccts a vector containing pointers to various library objects that implement different functionalities.
     */
    void init(BoardProfile *pBoard, std::vector<class VHBaseType *> LibraryObjeccts);
    /**
     * @brief Playes a list of effects on the haptic device, in all available channels.
     *
     * @param effectList List of effects to be played.
     * @return int return VH_SUCCESS on success, VH_ERROR on failure
     */
    int play(std::initializer_list<std::unique_ptr<IVhEffect>> effectList);
    /**
     * @brief Playes a list of effects on the haptic device, in the specified channel.
     *
     * @param effectList List of effects to be played.
     * @param channelNo     The channel number to play the effects on.
     *                      If set to 0 , the effects will be played on all available channels.
     *                      If set to a specific channel number (0, 1, 2, ...), the effects will be played only on that channel.
     *                      If set to a value greater than the number of available channels, the effects will not be played and an error will be returned.
     * @return int return VH_SUCCESS on success, VH_ERROR on failure
     */
    int play(std::initializer_list<std::unique_ptr<IVhEffect>> effectList, int channelNo);
    /**
     * @brief  Playes a list of effects on the haptic device, in the channels associated with the specified tags.
     *
     * @param effectList  List of effects to be played.
     * @param tags  A '&' or '|' separated string of tags associated with the channels to play the effects on.
     *           For example, "left&right" will play the effects on channels tagged with both "left" and "right",
     *          while "left|right" will play the effects on channels tagged with either "left" or "right".
     * @note If no channels are found with the specified tags, the effects will not be played and an error will be returned.
     * @return int
     */
    int play(std::initializer_list<std::unique_ptr<IVhEffect>> effectList, const char *tags);
    /**
     * @brief  Playes a list of effects on the haptic device, in the specified channels.
     *
     * @param effectList  List of effects to be played.
     * @param chnls A vector of channel numbers to play the effects on.
     *         For example, {0, 2} will play the effects on channels 0 and 2.
     * @return int return VH_SUCCESS on success, VH_ERROR on failure
     */
    int play(std::initializer_list<std::unique_ptr<IVhEffect>> effectList, std::vector<unsigned char> chnls);

    /**
     * @brief Play a single effect on the haptic device in all available channels.
     *
     * @param effect The effect to be played.
     * @return int return VH_SUCCESS on success, VH_ERROR on failure
     */
    int play(std::unique_ptr<IVhEffect> effect);
    /**
     * @brief Play a single effect on the haptic device in the specified channel.
     *
     * @param effect The effect to be played.
     * @param channelNo The channel number to play the effect on.
     *                  If set to 0 , the effect will be played on all available channels.
     *                  If set to a specific channel number (0, 1, 2, ...), the effect will be played only on that channel.
     *                  If set to a value greater than the number of available channels, the effect will not be played and an error will be returned.
     * @return int return VH_SUCCESS on success, VH_ERROR on failure
     */
    int play(std::unique_ptr<IVhEffect> effect, int channelNo);
    /**
     * @brief Play a single effect on the haptic device in the channels associated with the specified tags.
     *
     * @param effect The effect to be played.
     * @param tags A '&' or '|' separated string of tags associated with the channels to play the effect on.
     *             For example, "left&right" will play the effect on channels tagged with both "left" and "right",
     *             while "left|right" will play the effect on channels tagged with either "left" or "right".
     * @note If no channels are found with the specified tags, the effect will not be played and an error will be returned.
     * @return int return VH_SUCCESS on success, VH_ERROR on failure
     */
    int play(std::unique_ptr<IVhEffect> effect, const char *tags);
    /**
     * @brief Play a single effect on the haptic device in the specified channels.
     *
     * @param effect The effect to be played.
     * @param chnls A vector of channel numbers to play the effect on.
     *              For example, {0, 2} will play the effect on channels 0 and 2.
     * @return int return VH_SUCCESS on success, VH_ERROR on failure
     */
    int play(std::unique_ptr<IVhEffect> effect, std::vector<unsigned char> chnls);
    /**
     * @brief Queues a list of effects to be played on the haptic device in the specified channels.
     * This function will not play the effects until the renderQueue function is called.
     *
     * @param effectList  List of effects to be queued.
     * @param oper The operation type for queuing effects.
     *              It can be either APPEND_EFFECT or REPLACE_EFFECT (default is APPEND_EFFECT).
     *             - APPEND_EFFECT: Adds the effects to the end of the current queue.
     *            - REPLACE_EFFECT: Clears the current queue and adds the new effects.
     * @return int return VH_SUCCESS on success, VH_ERROR on failure
     */
    int queue(std::initializer_list<std::unique_ptr<IVhEffect>> effectList, QueueOperType oper = APPEND_EFFECT);
    /**
     * @brief Queues a list of effects to be played on the haptic device in the specified channel.
     * This function will not play the effects until the renderQueue function is called.
     *
     * @param effectList  List of effects to be queued.
     * @param channelNo The channel number to queue the effects on.
     *                  If set to 0 , the effects will be queued on all available channels.
     *                  If set to a specific channel number (0, 1, 2, ...), the effects will be queued only on that channel.
     *                  If set to a value greater than the number of available channels, the effects will not be queued and an error will be returned.
     * @param oper The operation type for queuing effects.
     *              It can be either APPEND_EFFECT or REPLACE_EFFECT (default is APPEND_EFFECT).
     *             - APPEND_EFFECT: Adds the effects to the end of the current queue.
     *            - REPLACE_EFFECT: Clears the current queue and adds the new effects.
     * @return int return VH_SUCCESS on success, VH_ERROR on failure
     */
    int queue(std::initializer_list<std::unique_ptr<IVhEffect>> effectList, int channelNo, QueueOperType oper = APPEND_EFFECT);
    /**
     * @brief Queues a list of effects to be played on the haptic device in the channels associated with the specified tags.
     * This function will not play the effects until the renderQueue function is called.
     *
     * @param effectList  List of effects to be queued.
     * @param tags A '&' or '|' separated string of tags associated with the channels to queue the effects on.
     *             For example, "left&right" will queue the effects on channels tagged with both "left" and "right",
     *             while "left|right" will queue the effects on channels tagged with either "left" or "right".
     * @param oper The operation type for queuing effects.
     *              It can be either APPEND_EFFECT or REPLACE_EFFECT (default is APPEND_EFFECT).
     *             - APPEND_EFFECT: Adds the effects to the end of the current queue.
     *            - REPLACE_EFFECT: Clears the current queue and adds the new effects.
     * @note If no channels are found with the specified tags, the effects will not be queued and an error will be returned.
     * @return int return VH_SUCCESS on success, VH_ERROR on failure
     */
    int queue(std::initializer_list<std::unique_ptr<IVhEffect>> effectList, const char *tags, QueueOperType oper = APPEND_EFFECT);
    /**
     * @brief Queues a list of effects to be played on the haptic device in the channels associated with the specified tags.
     * This function will not play the effects until the renderQueue function is called.
     * @param effectList List of effects to be queued.
     * @param chnls A vector of channel numbers to queue the effects on.
     *              For example, {0, 2} will queue the effects on channels 0 and 2.
     * @param oper The operation type for queuing effects.
     *              It can be either APPEND_EFFECT or REPLACE_EFFECT (default is APPEND_EFFECT).
     *             - APPEND_EFFECT: Adds the effects to the end of the current queue.
     *            - REPLACE_EFFECT: Clears the current queue and adds the new effects.
     * @return int return VH_SUCCESS on success, VH_ERROR on failure
     */
    int queue(std::initializer_list<std::unique_ptr<IVhEffect>> effectList, std::vector<unsigned char> chnls, QueueOperType oper = APPEND_EFFECT);
    /**
     * @brief Check the queue is empty for the given channel number
     *
     * @param chnlNo channel number or 0
     * @note
     * if chnlNo is 0, it checks all channels and returns true only if all queues are empty.
     * If chnlNo is a specific channel number, it checks only that channel's queue.
     *
     * @return true
     * @return false
     */
    bool queueIsEmpty(int chnlNo);
    /**
     * @brief Playes the effects that are queued in the specified channel.
     *
     * This function processes and plays the effects that have been previously queued using the `queue` method.
     * If no channel number is specified (or if set to 0), it will render the queues for all available channels.
     * If a specific channel number is provided, it will render the queue only for that channel.
     *
     * @param chnlNo  The channel number to render the queued effects on.
     * @return int Status code: -1 for error, 0 for success, 1 for empty queue
     */
    int renderQueue(int chnlNo = 0);
    /**
     * @brief Stop the currently playing effects on all channels or a specific channel.
     *
     * This function stops any effects that are currently being played on the haptic device.
     *
     * @param milliseconds maximum time to wait for the effects to stop before returning. Default is 0 (no wait).
     */
    void stop(unsigned int milliseconds = 0);
    /**
     * @brief Get the Board Uniqueue name
     *
     * @return std::string
     */
    std::string getBoardUniqName();
    /**
     * @brief Get the Board Ptr object
     *
     * @return BoardProfile*
     */
    BoardProfile *getBoardPtr();
    /**
     * @brief Set the Verbosity Level
     *
     * @param vals verbosity level: SILENT, ERROR_ONLY, INFO_ONLY, WARNING_ONLY, or VERBOSE (combine with |)
     */
    void setVerbosity(int vals);
    /**
     * @brief Set the Device Information
     *
     * @param DeviceName  Device Name . Default is "VHDevice"
     * @param Manufacture Manufacture Name. Default is "UNDEFINED"
     * @param ModelNo     Model Number. Default is "VH_GENERIC_MODEL"
     * @param SerialNo    Serial Number. Default is "0000000000"
     * @param VersionNo   Version Number. Default is Latest Version
     */
    void setDeviceInfo(
        std::string DeviceName = "VHDevice",
        std::string Manufacture = "UNDEFINED",
        std::string ModelNo = "VH_GENERIC_MODEL",
        std::string SerialNo = "0000000000",
        std::string VersionNo = VERSION_STRING);
    /**
     * @brief Retrives the saved device name from the board's non-volatile memory.
     *
     * @param pBoard Pointer to the board profile from which to retrieve the device name.
     * @return std::string
     */
    std::string getSavedDeviceName(BoardProfile *pBoard);
    /**
     * @brief Get the Instance object
     *
     * @return VectorHaptics*
     */
    static VectorHaptics *getInstance();
    /**
     * @brief log messages to the internal message queue.
     *
     * @param msg message string
     * @param type  message type (LOG_TYPE::LOG_ERROR, LOG_TYPE::LOG_WARNING, LOG_TYPE::LOG_INFO)
     */
    void logMessages(std::string msg, LOG_TYPE type);
    void setPcmCallBack(PcmchunkCb cb);
    /**
     * @brief Execute a string command.
     *
     * @param comdList The command string to be executed. "p 1 10;t 1 10;"
     *
     * @code {.cpp}
     * executeStringCommand("p 1 10;t 1 10;");
     * // This command will play a pulse effect with intensity 1 and duration 10ms,
     * //followed by a tick effect with intensity 1 and duration 10ms.
     * @endcode
     *
     */
    void executeStringCommand(std::string comdList);
    /**
     * @brief Check if there are any messages in the internal message queue.
     *
     * @return int returns the number of messages in the queue.
     */
    int hasMessage();
    /**
     * @brief Retrieve and remove the oldest message from the internal message queue.
     *
     * @return const char* returns a pointer to the message string. Returns nullptr if no messages are available.
     */
    const char *popMessage();
    /**
     * @brief Execute a binary command
     *
     * @param data binary command data
     * @param len length of the data
     */
    void executeCommand(const unsigned char *data, unsigned short len);
    void delayifNeeded();
    /**
     * @brief Retrieves the current playback status of the VectorHaptics device.
     *
     * This function checks the playback status of all available haptic channels
     * and returns the overall device playback state. It returns `VHPlaybackState::VH_PLAYING`
     * as soon as any one of the channels is actively playing. If no channels are playing,
     * it returns `VH_IDLE`,
     *
     * This method does not guarantee that *all* channels are playing — only that at
     * least one channel is active if the result is `VH_PLAYING`.
     *
     * @return VHPlaybackState - The playback state of the device:
     * - `VH_PLAYING`: At least one channel is actively playing.
     * - `VH_IDLE`: No channel is playing
     * - `VH_DISABLED`: This effect is disabled.
     *
     * @note If no channels are available, the default return value is `VH_IDLE`.
     *
     * Please check the Example @ref playback_status.ino "playback_status"
     */
    VHPlaybackState playbackStatus();
    /**
     * @brief return the current playback status of the given channel
     * @param channelNo the channel number to check the playback status
     * @return VHPlaybackState - The playback state of the channel:
     * - `VH_PLAYING`: Channel is actively playing.
     * - `VH_IDLE`: Channel is playing
     * - `VH_DISABLED`: This effect is disabled.
     *
     * Please check the Example @ref playback_status.ino "playback_status"
     */
    VHPlaybackState playbackStatus(uint8_t channelNo);

private:
    void effectReceiver();
    void init(IVHChannels *pChannels);
    void init(IEffectReceiver *pEff);
    void init(IDevTools *pDev);
    void init(II2SoundBus *pMix);
    void delayPrim(unsigned int msec);

    int playEffects(IVhEffect *effect, int chnl = -1, const char *tag = nullptr, bool blocking = true);
    int playEffects(IVhEffect *effect, std::vector<unsigned char> &chnls, bool blocking = true);
    void addEffectsInQueue(IVhEffect *effect, std::vector<unsigned char> &chnls);

    int pulse(uint16_t &duration, float intensity, float sharpness);
    int pulse(uint16_t &duration, float intensity, float sharpness, int channel);
    int pulse(uint16_t &duration, float intensity, float sharpness, const char *tag);
    int pulse(uint16_t &duration, float intensity, float sharpness, std::vector<unsigned char> &chnls);

    int tick(uint16_t &duration, float intensity, float sharpness);
    int tick(uint16_t &duration, float intensity, float sharpness, int channel);
    int tick(uint16_t &duration, float intensity, float sharpness, const char *tag);
    int tick(uint16_t &duration, float intensity, float sharpness, std::vector<unsigned char> &chnls);

    int pause(uint16_t &duration);
    int pause(uint16_t &duration, int channel);
    int pause(uint16_t &duration, const char *tag);
    int pause(uint16_t &duration, std::vector<unsigned char> &chnls);

    int pcm(VH_Pcm *pPcm, int channel);
    int pcm(VH_Pcm *pPcm, const char *tag);
    int pcm(VH_Pcm *pPcm, std::vector<unsigned char> &chnls);
    int pcm(VH_Pcm *pPcm);

    int vibrate(uint16_t &duration, float strength, float frequency, float sharpness);
    int vibrate(uint16_t &duration, float strength, float frequency, float sharpness, int channel);
    int vibrate(uint16_t &duration, float strength, float frequency, float sharpness, const char *tag);
    int vibrate(uint16_t &duration, float strength, float frequency, float sharpness, std::vector<unsigned char> &chnls);

    int erm(float intensity, uint16_t &duration);
    int erm(float intensity, uint16_t &duration, int channel);
    int erm(float intensity, uint16_t &duration, const char *tag);
    int erm(float intensity, uint16_t &duration, std::vector<unsigned char> &chnls);

    int sweep(float &duration, StartEndParam &strength, StartEndParam &frequency, StartEndParam &sharpness);
    int sweep(float &duration, StartEndParam &strength, StartEndParam &frequency, StartEndParam &sharpness, int channel);
    int sweep(float &duration, StartEndParam &strength, StartEndParam &frequency, StartEndParam &sharpness, const char *tag);
    int sweep(float &duration, StartEndParam &strength, StartEndParam &frequency, StartEndParam &sharpness, std::vector<unsigned char> &channels);

    int customPrim(ICustomPrim *prim, int channel);
    int customPrim(ICustomPrim *prim, std::vector<unsigned char> &chnls);
    int customPrim(ICustomPrim *prim, const char *tag);

    friend class EffectReceiver;
    friend class DevToolsBase;

    bool checkCustomPrimParameters(ICustomPrim *prim);

    std::string getChannelInfo();
    std::string getChannelInfo(int chnlNo);
    std::string getDeviceInfo();

    std::string getAsString(float intensity, float duration);

    std::string getAsString(uint16_t duration, float intensity, float sharpness);
    std::string getAsString(uint16_t duration, float intensity, float frequency, float sharpness);
    static void loopTask(void *args);

    template <typename T>
    bool checkStrenSharp(float &intensity, float &sharpness);

    template <typename T>
    bool checkFrequency(float &frequency);

    template <typename T>
    bool checkDuration(uint16_t &duration);
};

/**
 * @example ADF_example.ino
 * @example BoardProfile_example.ino
 * @example channel_number.ino
 * @example channel_tags.ino
 * @example I2s_example.ino
 * @example Mixing_Primitives_with_Streamed_haptics.ino
 * @example PcmPlayback.ino
 * @example blocking_playback.ino
 * @example non_blocking_playback.ino
 * @example playback_status.ino
 * @example basic_effects.ino
 * @example basic_effects_using_variable.ino
 * @example custom_parameters.ino
 * @example custom_primitives.ino
 * @example default_parameters.ino
 */
