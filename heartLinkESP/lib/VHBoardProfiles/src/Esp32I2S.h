#pragma once

#include <Arduino.h>
#include <BoardProfile.h>
#include <Interface.h>
#include <Utilities/VHUtilities.h>

class Esp32I2S : public IBoardI2sMan
{
    Esp32I2S();
    hw_timer_t *mTimer = nullptr;
    static Esp32I2S *mInstance;
    bool mbI2sRunning = false;
    QueueHandle_t m_queueHandle = nullptr;
    const int TIMER_FREQ = 40000000;
    I2sInfo mLastInfo;
    bool mHasConfig = false;

public:
    ~Esp32I2S();
    static Esp32I2S *getInstance();
    void begin(I2sInfo &info) override;
    void stop() override;
    void start() override;
    void write(void *data, size_t size) override;
    void write(uint8_t val) override;
    bool IsRunning() override;

private:
    void writeI2s();
};
