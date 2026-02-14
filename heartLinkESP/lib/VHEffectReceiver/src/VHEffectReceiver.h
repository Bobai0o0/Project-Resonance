#pragma once
#include <string>
#include <Interface.h>

class EffectReceiver : public IEffectReceiver
{

private:
    unsigned long mBaud;
    unsigned char mRxtimeout;
    // IBoardSerial * mIBoardSerial;
    bool mbSerialProc;
    int m_iDataBuffSize, m_iCompilerPadding;
    static DataBufferManager mDataBuffManager;
    void dataParseProc();
    // size_t getCompilerPadding(size_t pcmDataLength);
    template <typename T>
    int payLoadParse(T *payLoad, size_t size);
    int processCmd(Header &header);

public:
    EffectReceiver();
    ~EffectReceiver();
    void init() override;
    void setDataBuffSize(int val) override;
    size_t getDataBuffSize() const;
    void disableSerialThread(bool Disable = true);
    int parseData(const unsigned char *data, unsigned short len) override;
};

/**
 * @class VHEffectReceiver
 * @brief Main API class for Vector Haptics Developer Tools.
 *
 * @code {.cpp}
 * VHEffectReceiver effectReceiver;
 * @endcode
 * <b>Check the following examples for usage:</b><br>
 *  @li @ref receiving_bin_cmds_8_update.ino "receiving_bin_cmds_8_update.ino"<br>
 *  @li @ref send_bin_cmds_8_update.py "send_bin_cmds_8_update.py"<br>
 *  @li @ref receiving_binary_commands.ino "receiving_binary_commands.ino"<br>
 *  @li @ref receiving_device_info.ino "receiving_device_info.ino"
 */

using VHEffectReceiver = EffectReceiver;

/**
 * @example receiving_bin_cmds_8_update.ino
 * @example send_bin_cmds_8_update.py
 * @example receiving_binary_commands.ino
 * @example receiving_device_info.ino
 *
 */