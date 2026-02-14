#pragma once

#include <Arduino.h>
#include <vector>
#include <BoardProfile.h>

#define CHNL_LEFT 25
#define CHNL_RIGHT 26
#define DEFAULT_PIN_CHNL 3

class Esp32Channel
{
    unsigned char m_pin;
    unsigned char m_Channel;

public:
    Esp32Channel(unsigned char pin, unsigned char channel)
        : m_pin(pin), m_Channel(channel) {}

    unsigned char getPin() { return m_pin; }
    unsigned char getChannel() { return m_Channel; }
};

class ESP32Profile : public BoardProfile
{
    unsigned char mOutPutPin = 0;
    uint32_t mPwmFrequency = PWM_FREQ;
    unsigned char mEsp32Channels = 0;
    std::vector<Esp32Channel> listChannelPins;
    static ESP32Profile *_mInstance;

    unsigned char getChannelFromPin(unsigned char pin);
    void initBasic();
    void writeBasic(unsigned char val);

public:
    ESP32Profile();
    static ESP32Profile *getInstance();
    ~ESP32Profile();

    void init(unsigned char pin = 0) override;
    void init(unsigned char pin1, unsigned char pin2, unsigned char ctrlPin = 0) override;
    void setOutput(unsigned char pin) override;
    void delay(unsigned int ms) override;
    void delayMicroseconds(unsigned int us) override;
    void write(unsigned char val) override;
    void writeSinglePin(unsigned char pin, unsigned char value) override;
    void writeDualPin(unsigned char pin1, unsigned char pin2, unsigned char val1, unsigned char val2) override;
    std::string GetMacIdEndChars() override;
    void logMessages(std::string msg, LOG_TYPE type = ERROR_MSG) override;
    void logError(int line, const char *file, const char *msg) override;
    void high(unsigned char pin) override;
    void low(unsigned char pin) override;
    IBoardMutex *createMutexHandle() override;
    IBoardQueue *createQueueHandle() override;
    unsigned long getTimeMicroseconds() override;
    unsigned long millis() override;
    int createTask(void (*func)(void *), const char *name, int stackSize, void *param, int priority, void *taskHandle) override;
    ITaskManager *createTask(TaskConfig *config) override;
    IBoardTimer *createTimerEvents(uint8_t timer, uint32_t freq, bool countup = true) override;
    IBoardI2sMan *getI2sMan() override;
    void restart() override;
    void save(const char *key, const char *value) override;
    std::string read(const char *key, const char *defaultVal) override;
    void pinMode(int pin, int mode) override;
    void pinMode(int pin) override { pinMode(pin, OUTPUT); }
    void digitalWrite(int pin, int val) override;
    int digitalRead(int pin) override;
    void changePwmFrequency(const unsigned char pin, const unsigned long freq) override;
};
