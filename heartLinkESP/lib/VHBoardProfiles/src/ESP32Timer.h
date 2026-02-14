#pragma once

#include <Arduino.h>
#include <BoardProfile.h>

class ESP32Timer : public IBoardTimer
{
public:
    ESP32Timer() = default;
    ESP32Timer(uint8_t timer, uint32_t freq, bool countup = true);
    ~ESP32Timer();

    void setCallback(TimerCb cb, void *param = nullptr) override;
    void startTimer() override;
    void stopTimer() override;

    void *getUserParam() const;
    TimerCb getCallBack() const;

private:
    void init();

    hw_timer_t *mTimerHandle = nullptr;
    uint8_t mTimerNo = 0;
    uint16_t mDivider = 2;
    TimerCb mTimerCb = nullptr;
    void *mUserParam = nullptr;
    uint64_t mTimerAlarmVal = 0;
private:
    static ESP32Timer *sTimers[4];
    static void IRAM_ATTR onTimer0();
    static void IRAM_ATTR onTimer1();
    static void IRAM_ATTR onTimer2();
    static void IRAM_ATTR onTimer3();
};
