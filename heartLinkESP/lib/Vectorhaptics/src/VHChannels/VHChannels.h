#pragma once
#include <VHChannels/VHChannel.h>
#include <Interface.h>

/**
 * @brief This class holds the channel information
 *
 * @code {.cpp}
 * VHChannels chanls;
 * chanls.add(1,{25});
 * @endcode
 *
 * <i> Please check the examples : </i><br>
 *      @li @ref channel_number.ino "channel_number"
 *      @li @ref channel_tags.ino "channel_tags"
 */
class VHChannels : public IVHChannels
{
    std::vector<VHChannel> m_pChannelList;
    void addPrim(Primitive &prim, const unsigned char &chnlNo);
    void addPrimNotPlay(Primitive &prim, const unsigned char &chnlNo);
    class IBoardMutex *mMutex;

public:
    /**
     * @brief Get the number of channels available in the VHChannels object.
     *
     * @return int Number of channels.
     */
    int getCount() override;
    /**
     * @brief Get the Channel pointer by the index in the channel list.
     *
     * @param idx chanel index (0 to getCount()-1)
     * @return VHChannel*
     */
    VHChannel *getChnlByIndex(int idx) override;
    /**
     * @brief Initialize all channels in the VHChannels object.
     *
     * @note This function will call when you pass the VHChannels object to VectorHaptics::init
     *
     */
    void init() override;
    /**
     * @brief Register a DSP callback function for a specific channel.
     *
     * @param channelNo channel number to register the DSP callback
     * @param dspCb Function pointer to the DSP callback function that takes an unsigned char as input and returns an unsigned char.
     * @code {.cpp}
     * int dspFunction(unsigned char input)
     * {
     *      auto modifiedValue = input/2; // Example modification
     *      return modifiedValue;
     * }
     *
     * applyDSP(1, dspFunction); // Register the DSP function for channel 1
     * @endcode

     * <b> Check the below example :</b><br>
     * @ref dsp_callback.ino "dsp_callback.ino"
     *
     *
     */
    void applyDSP(int channelNo, std::function<uint8_t(uint8_t)> dspCb) override;
    /**
     * @brief Register a callback function for a specific single pin channel for specified channel status.
     *
     * @param chnlNo Channel number to register the callback
     * @param status The channel status to trigger the callback (e.g., CHANNEL_IDLE, CHANNEL_WAKE).
     * @param statusCb  Function pointer to the callback function that takes an unsigned char as input and returns void.
     *
     * <b> Check the below example :</b><br>
     * @ref chnl_callback_example_1.ino "chnl_callback_example_1"
     */
    void attachCallback(int chnlNo, CHANNEL_STATUS status, std::function<void(uint8_t)> statusCb);
    /**
     * @brief Register a callback function for a specific dual pin channel for specified channel status.
     *
     * @param chnlNo Channel number to register the callback
     * @param status The channel status to trigger the callback (e.g., CHANNEL_IDLE, CHANNEL_WAKE).
     * @param statusCb  Function pointer to the callback function that takes 2 unsigned char as input and returns void.
     *
     * <b> Check the below example :</b><br>
     * @ref chnl_callback_example_2.ino "chnl_callback_example_2"
     */
    void attachCallback(int chnlNo, CHANNEL_STATUS status, std::function<void(uint8_t, uint8_t)> statusCb);
    /**
     * @brief Construct a new VHChannels object
     * @code {.cpp}
     * VHChannels channels;
     * @endcode
     *
     */
    VHChannels();
    /**
     * @brief Enables or disables the idle handler for a specific channel.
     *
     * @param chnlNo channel number to enable/disable the idle handler
     * @param enable Set to true to enable the idle handler, false to disable it.
     */
    void enableIdleHandler(uint8_t chnlNo, bool enable);
    /**
     * @brief Addes a new channel to the VHChannels list.
     *
     * @param chnl channel number to assign
     * @param pins a list of pins to assign to the channel. For single pin channel,
     *              provide only one pin number. For dual pin channel, provide two pin numbers.
     * @note If the channel is an i2s channel then
     *         @li @c 1st pin = BCLK/WS
     *         @li @c 2nd pin = LRC/SCK
     *         @li @c 3rd pin = DIN/SDIN
     *
     * @param list A list channel tags to be assigned to the channel.
     * @param primOpt  Add the allowed primitive options for the channel.
     * @note If no options are provided, all primitives will be enabled by default.
     * @parblock You can provide multiple options by using bitwise OR operator.
     *          For example, to enable only PULSE and VIBRATE primitives, you can
     * @code {.cpp}
     * channels.add(1, {5}, {"left","low"}, {PULSE|VIBRATE});
     *   OR
     * channels.add(1, {5}, {"left","low"}, {PULSE, VIBRATE});
     * @endcode
     * @endparblock
     *
     * <b> Check the below example :</b><br>
     * @ref boolean_operation.ino "boolean_operation.ino"
     */
    void add(unsigned char chnl, std::vector<unsigned char> pins,
             std::initializer_list<std::string> list,
             std::vector<int> primOpt = {-1});
    /**
     * @brief Addes a new channel to the VHChannels list.
     *
     * @param chnl channel number to assign
     * @param pins a list of pins to assign to the channel. For single pin channel,
     *              provide only one pin number. For dual pin channel, provide two pin numbers.
     * @note If the channel is an i2s channel then
     *         @li @c 1st pin = BCLK/WS
     *         @li @c 2nd pin = LRC/SCK
     *         @li @c 3rd pin = DIN/SDIN
     *
     * @param primOpt  Add the allowed primitive options for the channel.
     * @note If no options are provided, all primitives will be enabled by default.
     * @parblock You can provide multiple options by using bitwise OR operator.
     *          For example, to enable only PULSE and VIBRATE primitives, you can
     * @code {.cpp}
     * channels.add(1, {5}, {"left","low"}, {PULSE|VIBRATE});
     *   OR
     * channels.add(1, {5}, {"left","low"}, {PULSE, VIBRATE});
     * @endcode
     * @endparblock
     *
     * <b> Check the below example :</b><br>
     * @ref boolean_operation.ino "boolean_operation.ino"
     */
    void add(unsigned char chnl, std::vector<unsigned char> pins, std::vector<int> primOpt = {-1});
    /**
     * @brief Get the current playback status of a specific channel.
     *
     * @param channel  The channel number to check the playback status.
     * @return VHPlaybackState
     * @note VHPlaybackState Enum values:
     *      @li @c VH_IDLE: Playback is idle
     *      @li @c VH_PLAYING: Playback is in progress
     *      @li @c VH_DISABLED: Playback is disabled
     *      @li @c VH_UNKNOWN: Unknown state
     *
     */
    VHPlaybackState playbackStatus(int channel) override;
    /**
     * @brief
     *
     * @param chnlNo
     * @param freqHz
     */
    void changePwmFrequency(const unsigned char chnlNo, const unsigned int freqHz = PWM_FREQ) override;

    virtual void addPrimitiveRenderQueue(Primitive prim, unsigned char chnlNo) override;
    virtual void addPrimitiveRenderQueue(Primitive prim, const char *tags) override;
    virtual void addPrimitiveRenderQueue(Primitive prim, std::vector<unsigned char> &chnl) override;

private:
    void addPrimitive(Primitive prim, unsigned char chnlNo) override;
    void addPrimitive(Primitive prim, const char *tags) override;
    void addPrimitive(Primitive prim, std::vector<unsigned char> &chnl) override;
    int getOptions(std::vector<int> &primOpt);
    virtual VHChannel *getChannel(int chnlNum) override;
    virtual void getChnlByTag(const char *tag, std::vector<unsigned char> &res) override;
    void getAllChnls(std::vector<unsigned char> &res) override;
    VHChannel *getDuelChnl() override;
    std::string toString() override;

    bool queueIsEmpty(unsigned char chnlNo) override;

    void clearRenderQueue(unsigned char chnlNo) override;
    void clearPrimitives() override;
    void setPrimManagerPtr(IPrimitiveManager *ptr) override;
};

class VectorHaptics;
class VHChannel;
enum class MSG_TYPE
{
    SUCCESS,
    ERROR,
    INFO
};

class Parser
{
    std::vector<unsigned char> mActiveChannels;

    VHChannels *vhChannelList;
    VectorHaptics *mVhPtr;

    // Helper function to check if a channel satisfies the operands and operators
    bool checkChannelConditions(VHChannel *chnl, const std::vector<std::string> &operands, const std::vector<std::string> &operators);

    // Helper function to handle the logic for multiple operands and operators
    void handleOperands(const std::vector<std::string> &separatedStrings);

    // Helper function to send messages
    void vSendMessage(const std::string &msg, MSG_TYPE type = MSG_TYPE::SUCCESS);

    // Helper function to activate channels
    void activateChannel(const std::string &msg);

    // Helper function to trim and check if a string is empty
    bool isEmpty(const std::string &val);

public:
    Parser(VHChannels *vhChnl, VectorHaptics *vh);
    ~Parser();

    std::string reFormat(const std::string &com, std::vector<std::string> &cmdList);
    void parseCommand(std::vector<std::string> &separatedStrings);
    void getActiveChannels(std::vector<unsigned char> &res);
};

/**
 * @example dsp_callback.ino
 * @example chnl_callback_example_1.ino
 * @example chnl_callback_example_2.ino
 * @example boolean_operation.ino
 */