#pragma once
#include <Interface.h>
#include <vector>
#include <regex>

#define IS_NUMBER(x)                                                                           \
    for (size_t i = 1; i < cmds.size(); i++)                                                   \
    {                                                                                          \
        if (!std::regex_match(cmds[i], float_pattern))                                         \
        {                                                                                      \
            mVhPtr->logMessages("Only numeric parameters are allowed  ", LOG_TYPE::ERROR_MSG); \
            return VH_ERROR;                                                                   \
        }                                                                                      \
    }

class DevToolsBase : public IDevTools
{
    std::string m_strLastCmd;
    std::string mChnlTag;
    std::vector<unsigned char> mChnls;
    int mChannel = 0;
    bool mInit = false;

public:
    DevToolsBase();
    ~DevToolsBase();
    void init();

private:
    std::regex float_pattern;
    int parseCommandList(std::string &strCmd) override;
    void clear();
    bool isEmpty(std::string &val);
    int parseCommand(std::string &com);
    bool reparseCommand(std::string &command);
    int runCommand(std::vector<std::string> &cmds);
    int runHdiCommand(std::string &strChannel);
    int splitStringToArray(std::string &strArray, unsigned char *array, unsigned int length);
};

/**
 * @class VHDevTools
 * @brief Main API class for Vector Haptics Developer Tools.
 *
 * @code {.cpp}
 * VHDevTools devTools;
 * @endcode
 * <b>Check the following examples for usage:</b><br>
 * @ref debug_code.ino "debug_code.ino" <br>
 * @ref serial_cmd_uart.ino "serial_cmd_uart.ino"
 *
 * @example debug_code.ino
 * @example serial_cmd_uart.ino
 *
 */
using VHDevTools = DevToolsBase;