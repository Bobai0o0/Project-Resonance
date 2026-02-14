#pragma once

#include <Arduino.h>
#include <BoardProfile.h>

class Esp32MutexHandle : public IBoardMutex
{
    SemaphoreHandle_t xMutex = nullptr;

public:
    Esp32MutexHandle();
    ~Esp32MutexHandle();
    void *createMutex() override;
    bool take(uint32_t xBlockTime = 0) override;
    void give() override;
};
