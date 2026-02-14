#pragma once

#include <Arduino.h>
#include <BoardProfile.h>

class Esp32QueueHandle : public IBoardQueue
{
    QueueHandle_t m_queueHandle = nullptr;

public:
    Esp32QueueHandle();
    ~Esp32QueueHandle();
    void createQueue(const unsigned int queueSize, const unsigned int size) override;
    int receive(void *const pvBuffer, unsigned int xTicksToWait) override;
    int send(const void *const pvItemToQueue, unsigned int xTicksToWait) override;
    int receiveISR(void *const pvBuffer, int *pxTaskWoken) override;
    int sendISR(const void *const pvItemToQueue, int *pxTaskWoken) override;
    int hasItems() override;
};
